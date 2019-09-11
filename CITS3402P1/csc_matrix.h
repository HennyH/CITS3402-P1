#pragma once

#include "matrix.h"

struct csc_matrix
{
  int n_non_zeros;
  struct matrix* matrix;
};

struct csc_matrix* csc_matrix_constructor(int width, int height, int* values);

int* csc_matrix_get_col(int col, struct csc_matrix* csc_matrix);
