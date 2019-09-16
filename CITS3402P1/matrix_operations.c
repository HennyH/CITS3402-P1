#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"
#include "matrix_operations.h"

enum mop_errno_t matrix_scalar_multiply(char data_type, float a, void* matrix, int width, int height, matrix_get_col get_col, matrix_constructor constructor, void** result_matrix)
{
  union matrix_value* result_values = (int*)malloc(sizeof(union matrix_value) * width * height);

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
        set_zero_matrix_value(DATA_TYPE_FLOAT, &result_values[col_i * height + row_i]);
        result_values[col_i * width + row_i].f = a * (data_type == DATA_TYPE_INTEGER ? column[row_i].i : column[row_i].f);
      }
    }
  }

  *result_matrix = constructor('f', width, height, result_values);
  free(result_values);
  return mop_errno_ok;
}

enum mop_errno_t matrix_trace(char data_type, struct coo_matrix* coo_matrix, union matrix_value* trace)
{
  int i;
  float trace_f = 0;
  int trace_i = 0;
  int n_values = coo_matrix->n_triples;
#pragma omp parallel for shared(coo_matrix) reduction(+: trace_f) reduction(+: trace_i)
  for (i = 0; i < n_values; i++) {
    if (coo_matrix->triples[i].row_i == coo_matrix->triples[i].col_i) {
      if (data_type == DATA_TYPE_INTEGER) {
        trace_i += coo_matrix->triples[i].value.i;
      }
      else {
        trace_f += coo_matrix->triples[i].value.f;
      }
    }
  }

  if (data_type == DATA_TYPE_FLOAT) {
    trace->f = trace_f;
  }
  else {
    trace->i = trace_i;
  }
  return mop_errno_ok;
}

enum mop_errno_t matrix_add(char left_data_type, char right_data_type, int width, int height, void* left_matrix, void* right_matrix, matrix_get_col get_col, matrix_constructor constructor, void** result_matrix)
{
  union matrix_value* result_values = (union matrix_value*)malloc(sizeof(union matrix_value) * width * height);
  const char result_data_type = left_data_type == DATA_TYPE_FLOAT || right_data_type == DATA_TYPE_FLOAT ? DATA_TYPE_FLOAT : DATA_TYPE_INTEGER;

  for (int col_i = 0; col_i < width; col_i++)
  {
    union matrix_value* left_column;
    union matrix_value* right_column;
#pragma omp parallel shared(left_column, right_column, result_values)
    {
#pragma omp single
      {
        left_column = get_col(col_i, left_matrix);
        right_column = get_col(col_i, right_matrix);
      }
      int row_i = 0;
#pragma omp for
      for (row_i = 0; row_i < height; row_i++) {
        set_zero_matrix_value(result_data_type, &result_values[col_i * height + row_i]);
        if (result_data_type == DATA_TYPE_FLOAT) {
          result_values[col_i * height + row_i].f =
            (right_data_type == DATA_TYPE_FLOAT ? left_column[row_i].f : left_column[row_i].i) *
            (left_data_type == DATA_TYPE_FLOAT ? right_column[row_i].f : right_column[row_i].i);
        }
        else {
          result_values[col_i * height + row_i].i =
            (right_data_type == DATA_TYPE_FLOAT ? left_column[row_i].f : left_column[row_i].i) *
            (left_data_type == DATA_TYPE_FLOAT ? right_column[row_i].f : right_column[row_i].i);
        }
      }
    }
  }

  *result_matrix = constructor(
    left_data_type == DATA_TYPE_FLOAT || right_data_type == DATA_TYPE_FLOAT ? DATA_TYPE_FLOAT : DATA_TYPE_INTEGER,
    width,
    height,
    result_values
  );
  free(result_values);
  return mop_errno_ok;
}

enum mop_errno_t matrix_multiply(
  char left_data_type,
  int left_matrix_width,
  int left_matrix_height,
  void* left_matrix,
  matrix_get_row get_left_matrix_row,
  char right_data_type,
  int right_matrix_width,
  int right_matrix_height,
  void* right_matrix,
  matrix_get_col get_right_matrix_col,
  matrix_constructor constructor,
  void** result_matrix
) {
  if (right_matrix_height != left_matrix_width) {
    return mop_errno_dimension;
  }
  const int result_matrix_width = right_matrix_width;
  const int result_matrix_height = left_matrix_height;
  const char result_data_type = left_data_type == DATA_TYPE_FLOAT || right_data_type == DATA_TYPE_FLOAT ? DATA_TYPE_FLOAT : DATA_TYPE_INTEGER;
  union matrix_value* result_values = malloc(sizeof(union matrix_value) * result_matrix_width * result_matrix_height);
  union matrix_value* left_matrix_row;

  for (int right_matrix_col_i = 0; right_matrix_col_i < right_matrix_width; right_matrix_col_i++) {
    union matrix_value* right_matrix_column = get_right_matrix_col(right_matrix_col_i, right_matrix);
    int left_matrix_row_i;
    int i;
#pragma omp parallel for shared(right_matrix_column, result_values) private(left_matrix_row, i)
    for (left_matrix_row_i = 0; left_matrix_row_i < left_matrix_height; left_matrix_row_i++) {
      left_matrix_row = get_left_matrix_row(left_matrix_row_i, left_matrix);
      union matrix_value col_row_product_sum = { .i = 0,.f = 0 };
      for (int i = 0; i < right_matrix_height; i++) {
        if (result_data_type == DATA_TYPE_FLOAT) {
          col_row_product_sum.f +=
            (right_data_type == DATA_TYPE_FLOAT ? right_matrix_column[i].f : right_matrix_column[i].i) *
            (left_data_type == DATA_TYPE_FLOAT ? left_matrix_row[i].f : left_matrix_row[i].i);
        }
        else {
          col_row_product_sum.i +=
            (right_data_type == DATA_TYPE_FLOAT ? right_matrix_column[i].f : right_matrix_column[i].i) *
            (left_data_type == DATA_TYPE_FLOAT ? left_matrix_row[i].f : left_matrix_row[i].i);
        }
      }
      result_values[result_matrix_width * left_matrix_row_i + right_matrix_col_i] = col_row_product_sum;
    }
  }

  *result_matrix = constructor(
    result_data_type,
    result_matrix_width,
    result_matrix_height,
    result_values
  );
  free(result_values);
  return mop_errno_ok;
}