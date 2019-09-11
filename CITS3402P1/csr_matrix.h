#pragma once

#include "matrix.h"

struct csr_matrix
{
  int n_non_zeros;
  struct matrix* matrix;
};

struct csr_matrix* csr_matrix_constructor(int width, int height, int* values);

int* csr_matrix_get_row(int row, struct csr_matrix* csr_matrix);
