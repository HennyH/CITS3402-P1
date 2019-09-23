#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>
#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"
#include "matrix_operations.h"

enum mop_errno_t matrix_scalar_multiply(char data_type, double a, void* matrix, int width, int height, matrix_get_col get_col, matrix_constructor constructor, void** result_matrix, char* result_data_type, clock_t* elapsed)
{
  union matrix_value* result_values = (union matrix_value*)calloc(width * height, sizeof(union matrix_value));

  clock_t start = clock();

  for (int col_i = 0; col_i < width; col_i++)
  {
    union matrix_value* column;
#pragma omp parallel shared(column, width, height, result_values)
    {
#pragma omp single
      column = get_col(col_i, matrix);
      int row_i = 0;
#pragma omp for
      for (row_i = 0; row_i < height; row_i++) {
        union matrix_value cell_value = { .d = a * (data_type == DATA_TYPE_INTEGER ? (double)column[row_i].i : column[row_i].d) };
        set_ltr_ttb_value(row_i, col_i, width, height, cell_value, result_values);
      }
    }
  }

  clock_t end = clock();
  *elapsed = end - start;

  *result_matrix = constructor('f', width, height, result_values);
  *result_data_type = DATA_TYPE_DOUBLE;
  free(result_values);
  return mop_errno_ok;
}

enum mop_errno_t matrix_trace(char data_type, struct coo_matrix* coo_matrix, union matrix_value* trace, char* result_data_type, clock_t* elapsed)
{
  int i;
  double trace_d = 0;
  int trace_i = 0;
  int n_values = coo_matrix->n_triples;

  clock_t start = clock();

#pragma omp parallel for shared(coo_matrix) reduction(+: trace_d) reduction(+: trace_i)
  for (i = 0; i < n_values; i++) {
    if (coo_matrix->triples[i].row_i == coo_matrix->triples[i].col_i) {
      if (data_type == DATA_TYPE_INTEGER) {
        trace_i += coo_matrix->triples[i].value.i;
      }
      else {
        trace_d += coo_matrix->triples[i].value.d;
      }
    }
  }

  clock_t end = clock();
  *elapsed = end - start;

  if (data_type == DATA_TYPE_DOUBLE) {
    *result_data_type = DATA_TYPE_DOUBLE;
    trace->d = trace_d;
  }
  else {
    *result_data_type = DATA_TYPE_INTEGER;
    trace->i = trace_i;
  }
  return mop_errno_ok;
}

enum mop_errno_t matrix_transpose(char data_type, int width, int height, void* matrix, matrix_get_col get_col, matrix_get_row get_row, matrix_constructor constructor, void** result_matrix, char* result_data_type, clock_t* elapsed) {
  if (get_col == NULL && get_row == NULL) {
    printf("Transpose requires either `get_col` or `get_row` to be provided.");
    return mop_errno_argument_invalid;
  }

  union matrix_value* result_values = (union matrix_value*)calloc(width * height, sizeof(union matrix_value));

  clock_t start = clock();

  /* What's going on here is that we allow the user to provide either a method of providing ROWS xor COLUMNS.
   * depending on what they give we will either transpose the matrix via changing rows->columns xor columns->rows.
   * We allow this to improve memory access patterns on matrices that are significantly wider or taller than they are tall or wide
   * respectivley.
   */
  const bool is_columnwise = get_col != NULL;
  /* we use a/b in the loops to try avoid any confusion that i/j (which associate with horizontal/vertical) might bring. */
  const int a_max = is_columnwise ? width : height; /* if going columnwise the outer loop is over the columns */
  const int b_max = is_columnwise ? height : width; /* if going columnwise the inner loop is over the row values of a column */
  for (int a = 0; a < a_max; a++) {
    union matrix_value* column_or_row_values;
#pragma omp parallel shared(a_max, b_max, column_or_row_values, result_values)
    {
#pragma omp single
      {
        column_or_row_values = is_columnwise ? get_col(a, matrix) : get_row(a, matrix);
      }
      int b;
#pragma omp for
      for (b = 0; b < b_max; b++) {
        /* if we are going columnwise the 'row' of the input value is `b` (the column is `a` the outer loop), but since we are transposing
         * and that means swapping rows->columns/colums->rows we use `a`. */
        const int dest_row_i = is_columnwise ? a : b;
        /* this follows similar logic to the line above. */
        const int dest_col_i = is_columnwise ? b : a;
        /* note that we make sure to swap the height/width parameters because we're putting it into the transposed matrix! */
        set_ltr_ttb_value(dest_row_i, dest_col_i, height, width, column_or_row_values[b], result_values);
      }
    }
  }

  clock_t end = clock();
  *elapsed = end - start;

  /* note we swapped the height/width arguments here! */
  *result_data_type = data_type;
  *result_matrix = constructor(data_type, height, width, result_values);
  free(result_values);
  return mop_errno_ok;
}

enum mop_errno_t matrix_add(char left_data_type, int left_matrix_width, int left_matrix_height, void* left_matrix, matrix_get_col get_left_matrix_col, char right_data_type, int right_matrix_width, int right_matrix_height, void* right_matrix, matrix_get_col get_right_matrix_col, matrix_constructor constructor, void** result_matrix, char* result_data_type, clock_t* elapsed)
{
  if (left_matrix_width != right_matrix_width || left_matrix_height != right_matrix_height) {
    return mop_errno_dimension_incompatible;
  }

  const int result_matrix_width = left_matrix_width;
  const int result_matrix_height = left_matrix_height;
  union matrix_value* result_values = (union matrix_value*)calloc(result_matrix_width * result_matrix_height, sizeof(union matrix_value));
  *result_data_type = left_data_type == DATA_TYPE_DOUBLE || right_data_type == DATA_TYPE_DOUBLE ? DATA_TYPE_DOUBLE : DATA_TYPE_INTEGER;

  clock_t start = clock();

  for (int col_i = 0; col_i < result_matrix_width; col_i++)
  {
    union matrix_value* left_column;
    union matrix_value* right_column;
#pragma omp parallel shared(left_column, right_column, result_values)
    {
#pragma omp single
      {
        left_column = get_left_matrix_col(col_i, left_matrix);
        right_column = get_right_matrix_col(col_i, right_matrix);
      }
      int row_i = 0;
#pragma omp for
      for (row_i = 0; row_i < result_matrix_height; row_i++) {
        union matrix_value col_row_sum = { .i = 0, .d = 0 };
        if (*result_data_type == DATA_TYPE_DOUBLE) {
          col_row_sum.d = (double)(left_data_type== DATA_TYPE_DOUBLE ? left_column[row_i].d : left_column[row_i].i) +
              (double)(right_data_type == DATA_TYPE_DOUBLE ? right_column[row_i].d : right_column[row_i].i);
        }
        else {
          /* if the result type is 'int' then we must be adding two integer matrices! */
          col_row_sum.i = left_column[row_i].i + right_column[row_i].i;
        }

        set_ltr_ttb_value(row_i, col_i, result_matrix_width, result_matrix_height, col_row_sum, result_values);
      }
    }
  }

  clock_t end = clock();
  *elapsed = end - start;

  *result_matrix = constructor(*result_data_type, result_matrix_width, result_matrix_height, result_values);
  free(result_values);
  return mop_errno_ok;
}

enum mop_errno_t matrix_multiply(char left_data_type, int left_matrix_width, int left_matrix_height, void* left_matrix, matrix_get_row get_left_matrix_row, char right_data_type, int right_matrix_width, int right_matrix_height, void* right_matrix, matrix_get_col get_right_matrix_col, matrix_constructor constructor, void** result_matrix, char* result_data_type, clock_t* elapsed) {
  if (right_matrix_height != left_matrix_width) {
    return mop_errno_dimension_incompatible;
  }
  
  const int result_matrix_width = right_matrix_width;
  const int result_matrix_height = left_matrix_height;
  *result_data_type = left_data_type == DATA_TYPE_DOUBLE || right_data_type == DATA_TYPE_DOUBLE ? DATA_TYPE_DOUBLE : DATA_TYPE_INTEGER;
  union matrix_value* result_values = malloc(sizeof(union matrix_value) * result_matrix_width * result_matrix_height);
  union matrix_value* left_matrix_row;

  clock_t start = clock();

  for (int right_matrix_col_i = 0; right_matrix_col_i < right_matrix_width; right_matrix_col_i++) {
    union matrix_value* right_matrix_column = get_right_matrix_col(right_matrix_col_i, right_matrix);
    int left_matrix_row_i;
    int i;
#pragma omp parallel for shared(right_matrix_column, result_values) private(left_matrix_row, i)
    for (left_matrix_row_i = 0; left_matrix_row_i < left_matrix_height; left_matrix_row_i++) {
      left_matrix_row = get_left_matrix_row(left_matrix_row_i, left_matrix);
      union matrix_value col_row_product_sum = { .i = 0,.d = 0 };
      for (int i = 0; i < right_matrix_height; i++) {
        if (*result_data_type == DATA_TYPE_DOUBLE) {
          col_row_product_sum.d +=
            (right_data_type == DATA_TYPE_DOUBLE ? right_matrix_column[i].d : right_matrix_column[i].i) *
            (left_data_type == DATA_TYPE_DOUBLE ? left_matrix_row[i].d : left_matrix_row[i].i);
        }
        else {
          col_row_product_sum.i +=
            (right_data_type == DATA_TYPE_DOUBLE ? right_matrix_column[i].d : right_matrix_column[i].i) *
            (left_data_type == DATA_TYPE_DOUBLE ? left_matrix_row[i].d : left_matrix_row[i].i);
        }
      }

      set_ltr_ttb_value(left_matrix_row_i, right_matrix_col_i, result_matrix_width, result_matrix_height, col_row_product_sum, result_values);
    }
  }

  clock_t end = clock();
  *elapsed = end - start;

  *result_matrix = constructor(*result_data_type, result_matrix_width, result_matrix_height, result_values);
  free(result_values);
  return mop_errno_ok;
}