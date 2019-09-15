#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "csc_matrix.h"

struct csc_matrix* csc_matrix_constructor(char data_type, int width, int height, void* values)
{
  int n_non_zeros = count_non_zero_values(data_type, width, height, values);

  int cnzs_len = width + 1;
  int rows_len = n_non_zeros;
  int vals_len = n_non_zeros;
  
  struct csc_matrix* m = (struct csc_matrix*)malloc(sizeof(struct csc_matrix));
  m->n_non_zeros = n_non_zeros;
  m->cnzs_len = cnzs_len;
  m->row_is_len = rows_len;
  m->vals_len = vals_len;
  m->width = width;
  m->height = height;
  m->data_type = data_type;
  m->cnzs = (int*)malloc(sizeof(int) * cnzs_len);
  m->row_is = (int*)malloc(sizeof(int) * rows_len);
  m->vals = (union matrix_value*)malloc(sizeof(union matrix_value) * vals_len);
  
  /* We prepend a 0 into the non-zeros by column to avoid erros
   * trying to retrieve col 1.
   */
  m->cnzs[0] = 0;

  int seen_non_zeros = 0;
  int next_value_i = 0;
  for (int col_i = 0; col_i < width; col_i++) {
    for (int row_i = 0; row_i < height; row_i++) {
      int i = col_i + row_i * width;
      if (is_zero_value(data_type, i, values)) {
        continue;
      }
      if (data_type == DATA_TYPE_INTEGER) {
        m->vals[next_value_i].i = ((int*)values)[i];
      }
      else {
        m->vals[next_value_i].f = ((float*)values)[i];;
      }
      
      m->row_is[next_value_i] = row_i;
      seen_non_zeros++;
      next_value_i++;
    }
    /* We have a `+1` here because we don't want to override the 0
     * we explicitly added in above before the loop! And on the
     * first iteration of the loop the `col` is 0!
     */
    m->cnzs[col_i + 1] = seen_non_zeros;
  }

  return m;
}

union matrix_value* csc_matrix_get_col(int col_i, struct csc_matrix* csc_matrix)
{
  if (col_i < 0) {
    printf("Expected column to be greater than equal to 1.");
    exit(1);
  }

  int n_non_zeros_before_col = csc_matrix->cnzs[col_i];
  int n_non_zeros_before_next_col = csc_matrix->cnzs[col_i + 1];

  union matrix_value* col_values = (int*)calloc(csc_matrix->height, sizeof(union matrix_value));
  for (int x = n_non_zeros_before_col; x < n_non_zeros_before_next_col; x++) {
    int row_index = csc_matrix->row_is[x];
    if (csc_matrix->data_type == DATA_TYPE_INTEGER) {
      col_values[row_index].i = ((int*)csc_matrix->vals)[x];
    }
    else {
      col_values[row_index].f = ((float*)csc_matrix->vals)[x];
    }
  }

  return col_values;
}