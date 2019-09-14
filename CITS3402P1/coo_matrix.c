#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "coo_matrix.h"

struct coo_matrix* coo_matrix_constructor(int width, int height, int* values)
{
  int n_non_zeros = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int value_i = j + i * width;
      int value = values[value_i];
      if (value == 0) {
        continue;
      }
      n_non_zeros++;
    }
  }

  struct coo_triple* triples = (struct coo_triple*)malloc(sizeof(struct coo_triple) * n_non_zeros);
  int next_triple_i = 0;
  for (int row_i = 0; row_i < height; row_i++) {
    for (int col_i = 0; col_i < width; col_i++) {
      int value_i = col_i + row_i * width;
      int value = values[value_i];
      if (value == 0) {
        continue;
      }
      triples[next_triple_i].row_i = row_i;
      triples[next_triple_i].col_i = col_i;
      triples[next_triple_i].value = value;
      next_triple_i++;
    }
  }

  struct coo_matrix* coo_matrix = (struct coo_matrix*)malloc(sizeof(struct coo_matrix));
  coo_matrix->n_triples = n_non_zeros;
  coo_matrix->triples = triples;
  coo_matrix->width = width;
  coo_matrix->height = height;
  return coo_matrix;
}