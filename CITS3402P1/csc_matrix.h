#pragma once

#include "matrix.h"

struct csc_matrix
{
  char data_type;
  int width;
  int height;
  int n_non_zeros;
  int cnzs_len;
  int row_is_len;
  int vals_len;
  int* cnzs;
  int* row_is;
  union matrix_value* vals;
};

struct csc_matrix* csc_matrix_constructor(char data_type, int width, int height, union matrix_value* values);

union matrix_value* csc_matrix_get_col(int col, struct csc_matrix* csc_matrix);

inline void csc_matrix_free(struct csc_matrix** csc_matrix)
{
  free((*csc_matrix)->cnzs);
  free((*csc_matrix)->row_is);
  free((*csc_matrix)->vals);
  free(*csc_matrix);
}