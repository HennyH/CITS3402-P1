#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"

void* matrix_scalar_multiply(int a, void* matrix, int width, int height, matrix_get_col get_col, matrix_constructor constructor);
int matrix_trace(struct coo_matrix* coo_matrix);
void* matrix_add(int width, int height, void* left_matrix, void* right_matrix, matrix_get_col get_col, matrix_constructor constructor);
void* matrix_multiply(
  int left_matrix_width,
  int left_matrix_height,
  void* left_matrix,
  matrix_get_row get_left_matrix_row,
  int right_matrix_width,
  int right_matrix_height,
  void* right_matrix,
  matrix_get_col get_right_matrix_col,
  matrix_constructor constructor
);