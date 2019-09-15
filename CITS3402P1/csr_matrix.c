#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "csr_matrix.h"

struct csr_matrix* csr_matrix_constructor(char data_type, int width, int height, union matrix_value* values)
{
  int n_non_zeros = count_non_zero_values(data_type, width, height, values);

  int cnzs_len = height + 1;
  int cols_len = n_non_zeros;
  int vals_len = n_non_zeros;

  struct csr_matrix* m = (struct csr_matrix*)malloc(sizeof(struct csr_matrix));
  m->n_non_zeros = n_non_zeros;
  m->cnzs_len = cnzs_len;
  m->col_is_len = cols_len;
  m->vals_len = vals_len;
  m->width = width;
  m->height = height;
  m->data_type = data_type;
  m->cnzs = (int*)malloc(sizeof(int) * cnzs_len);
  m->col_is = (int*)malloc(sizeof(int) * cols_len);
  m->vals = (union matrix_value*)malloc(sizeof(union matrix_value) * vals_len);

  /* We prepend a 0 into the non-zeros by column to avoid erros
   * trying to retrieve col 1.
   */
  m->cnzs[0] = 0;

  int seen_non_zeros = 0;
  int next_value_i = 0;
  for (int row_i = 0; row_i < height; row_i++) {
    for (int col_i = 0; col_i < width; col_i++) {
      int i = col_i + row_i * width;
      if (is_zero_value(data_type, i, values)) {
        continue;
      }

      m->vals[next_value_i] = values[i];
      m->col_is[next_value_i] = col_i;
      seen_non_zeros++;
      next_value_i++;
    }
    /* We have a `+1` here because we don't want to override the 0
     * we explicitly added in above! And on the first iteration of
     * the loop the `row` is 0!
     */
    m->cnzs[row_i + 1] = seen_non_zeros;
  }


  return m;
}

union matrix_value* csr_matrix_get_row(int row_i, struct csr_matrix* csr_matrix)
{
  if (row_i < 0) {
    printf("Expected row to be greater than equal to 1.");
    exit(1);
  }

  int n_non_zeros_before_row = csr_matrix->cnzs[row_i];
  int n_non_zeros_before_next_row = csr_matrix->cnzs[row_i+ 1];

  union matrix_value* row_values = (int*)calloc(csr_matrix->width, sizeof(union matrix_value));
  for (int x = n_non_zeros_before_row; x < n_non_zeros_before_next_row; x++) {
    int col_index = csr_matrix->col_is[x];
    if (csr_matrix->data_type == DATA_TYPE_INTEGER) {
      row_values[col_index].i = ((int*)csr_matrix->vals)[x];
    }
    else {
      row_values[col_index].f = ((float*)csr_matrix->vals)[x];
    }
  }

  return row_values;
}
