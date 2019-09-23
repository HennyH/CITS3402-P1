#pragma once

#include "matrix.h"

struct csr_matrix
{
  char data_type;
  int width;
  int height;
  int n_non_zeros;
  int cnzs_len;
  int col_is_len;
  int vals_len;
  int* cnzs;
  int* col_is;
  union matrix_value* vals;
};

struct csr_matrix* csr_matrix_constructor(char data_type, int width, int height, union matrix_value* values);

union matrix_value* csr_matrix_get_row(int row_i, struct csr_matrix* csr_matrix);

inline void csr_matrix_free(struct csr_matrix** csr_matrix)
{
  free((*csr_matrix)->cnzs);
  free((*csr_matrix)->col_is);
  free((*csr_matrix)->vals);
  free(*csr_matrix);
}
