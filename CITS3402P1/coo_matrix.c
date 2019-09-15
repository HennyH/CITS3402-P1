#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "coo_matrix.h"

struct coo_matrix* coo_matrix_constructor(char data_type, int width, int height, union matrix_value* values)
{
  int n_non_zeros = count_non_zero_values(data_type, width, height, values);

  struct coo_triple* triples = (struct coo_triple*)malloc(sizeof(struct coo_triple) * n_non_zeros);
  int next_triple_i = 0;
  for (int row_i = 0; row_i < height; row_i++) {
    for (int col_i = 0; col_i < width; col_i++) {
      int value_i = col_i + row_i * width;
      if (is_zero_value(data_type, value_i, values)) {
        continue;
      }
      triples[next_triple_i].row_i = row_i;
      triples[next_triple_i].col_i = col_i;
      triples[next_triple_i].value = values[value_i];
      next_triple_i++;
    }
  }

  struct coo_matrix* coo_matrix = (struct coo_matrix*)malloc(sizeof(struct coo_matrix));
  coo_matrix->data_type = data_type;
  coo_matrix->width = width;
  coo_matrix->height = height;
  coo_matrix->n_triples = n_non_zeros;
  coo_matrix->triples = triples;
  return coo_matrix;
}