#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "csc_matrix.h"

inline int csc_cnz_index(struct csc_matrix* csc_matrix, int i) {
  return i;
}

inline int csc_col_index(struct csc_matrix* csc_matrix, int i) {
  return (csc_matrix->matrix->width + 1) + i;
}

inline int csc_val_index(struct csc_matrix* csc_matrix, int i) {
  return (csc_matrix->matrix->width + 1) + (csc_matrix->n_non_zeros) + i;
}

struct csc_matrix* csc_matrix_constructor(int width, int height, int* values)
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

  int cnz_len = width + 1;
  int row_len = n_non_zeros;
  int val_len = n_non_zeros;
  int data_len = cnz_len + row_len + val_len;
  int* data = (int*)malloc(sizeof(int) * data_len);
  m->data = data;

  struct csc_matrix* csc_matrix = (struct csc_matrix*)malloc(sizeof(struct csc_matrix));
  csc_matrix->n_non_zeros = n_non_zeros;
  csc_matrix->matrix = m;

  /* We prepend a 0 into the non-zeros by column to avoid erros
   * trying to retrieve col 1.
   */
  data[csc_cnz_index(csc_matrix, 0)] = 0;

  int seen_non_zeros = 0;
  int next_value_i = 0;
  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      int i = col + row * width;
      int value = values[i];
      if (value == 0) {
        continue;
      }
      seen_non_zeros++;
      data[csc_col_index(csc_matrix, next_value_i)] = row;
      data[csc_val_index(csc_matrix, next_value_i)] = value;
      next_value_i++;
    }
    /* We have a `+1` here because we don't want to override the 0
     * we explicitly added in above! And on the first iteration of
     * the loop the `col` is 0!
     */
    data[csc_cnz_index(csc_matrix, col + 1)] = seen_non_zeros;
  }


  return csc_matrix;
}

int* csc_matrix_get_col(int col, struct csc_matrix* csc_matrix)
{
  int* data = csc_matrix->matrix->data;
  int n_non_zeros_before_col = data[csc_cnz_index(csc_matrix, col - 1)];
  int n_non_zeros_before_next_col = data[csc_cnz_index(csc_matrix, col)];

  int* col_values = (int*)calloc(csc_matrix->matrix->height, sizeof(int));
  for (int x = n_non_zeros_before_col; x < n_non_zeros_before_next_col; x++) {
    int row_index = data[csc_col_index(csc_matrix, x)];
    int value = data[csc_val_index(csc_matrix, x)];
    col_values[row_index] = value;
  }

  return col_values;
}