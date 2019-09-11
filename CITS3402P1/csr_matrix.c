#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

struct csr_matrix
{
  int n_non_zeros;
  struct matrix* matrix;
};

inline int csr_cnz_index(struct csr_matrix* csr_matrix, int i) {
  return i;
}

inline int csr_col_index(struct csr_matrix* csr_matrix, int i) {
  return (csr_matrix->matrix->height + 1) + i;
}

inline int csr_val_index(struct csr_matrix* csr_matrix, int i) {
  return (csr_matrix->matrix->height + 1) + (csr_matrix->n_non_zeros) + i;
}

struct csr_matrix* csr_matrix_constructor(int width, int height, int* values)
{
  struct matrix* m = (struct matrix*)malloc(sizeof(struct matrix));
  m->width = width;
  m->height = height;

  int n_non_zeros = 0;
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int i = col + row * width;
      int value = values[i];
      if (value == 0) {
        continue;
      }
      n_non_zeros++;
    }
  }

  int cnz_len = height + 1;
  int col_len = n_non_zeros;
  int val_len = n_non_zeros;
  int data_len = cnz_len + col_len + val_len;
  int* data = (int*)malloc(sizeof(int) * data_len);
  m->data = data;

  struct csr_matrix* csr_matrix = (struct csr_matrix*)malloc(sizeof(struct csr_matrix));
  csr_matrix->n_non_zeros = n_non_zeros;
  csr_matrix->matrix = m;

  /* We prepend a 0 into the non-zeros by row to avoid erros
   * trying to retrieve row 1.
   */
  data[csr_cnz_index(csr_matrix, 0)] = 0;

  int seen_non_zeros = 0;
  int next_value_i = 0;
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int i = col + row * width;
      int value = values[i];
      if (value == 0) {
        continue;
      }
      seen_non_zeros++;
      data[csr_col_index(csr_matrix, next_value_i)] = col;
      data[csr_val_index(csr_matrix, next_value_i)] = value;
      next_value_i++;
    }
    /* We have a `+1` here because we don't want to override the 0
     * we explicitly added in above! And on the first iteration of
     * the loop the `row` is 0!
     */
    data[csr_cnz_index(csr_matrix, row + 1)] = seen_non_zeros;
  }


  return csr_matrix;
}

int* csr_matrix_get_row(int row, struct csr_matrix* csr_matrix)
{
  int* data = csr_matrix->matrix->data;
  int n_non_zeros_before_row = data[csr_cnz_index(csr_matrix, row - 1)];
  int n_non_zeros_before_next_row = data[csr_cnz_index(csr_matrix, row)];

  int* row_values = (int*)calloc(csr_matrix->matrix->width, sizeof(int));
  for (int x = n_non_zeros_before_row; x < n_non_zeros_before_next_row; x++) {
    int column_index = data[csr_col_index(csr_matrix, x)];
    int value = data[csr_val_index(csr_matrix, x)];
    row_values[column_index] = value;
  }

  return row_values;
}