#pragma once

#include "matrix.h"

struct csr_matrix
{
  char data_type;
  int width;
  int height;
  int n_non_zeros;
  int* cnzs;
  int* cols;
  union matrix_value* vals;
};

struct csr_matrix* csr_matrix_constructor(char data_type, int width, int height, void* values);

union matrix_value* csr_matrix_get_row(int row_i, struct csr_matrix* csr_matrix);

