#pragma once

#include "matrix.h"

struct csc_matrix
{
  char data_type;
  int width;
  int height;
  int n_non_zeros;
  int* cnzs;
  int* rows;
  union matrix_value* vals;
};

struct csc_matrix* csc_matrix_constructor(char data_type, int width, int height, void* values);

union matrix_value* csc_matrix_get_col(int col, struct csc_matrix* csc_matrix);
