#pragma once

#include "matrix.h"

struct coo_triple
{
  int row_i;
  int col_i;
  union matrix_value value;
};


struct coo_matrix
{
  char data_type;
  int width;
  int height;
  int n_triples;
  struct coo_triple* triples;
};

struct coo_matrix* coo_matrix_constructor(char data_type, int width, int height, union matrix_value* values);

inline struct coo_triple coo_matrix_get_triple(struct coo_matrix* coo_matrix, int i)
{
  return ((struct coo_triple*)coo_matrix->triples)[i];
}
