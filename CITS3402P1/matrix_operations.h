#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"

void* matrix_scalar_multiply(
  char data_type,
  int a,
  void* matrix,
  int width,
  int height,
  matrix_get_col get_col,
  matrix_constructor constructor
);

int matrix_trace(char data_type, struct coo_matrix* coo_matrix);

void* matrix_add(
  char left_data_type,
  char right_data_type,
  int width,
  int height,
  void* left_matrix,
  void* right_matrix,
  matrix_get_col get_col,
  matrix_constructor constructor
);

void* matrix_multiply(
  char left_data_type,
  int left_matrix_width,
  int left_matrix_height,
  void* left_matrix,
  matrix_get_row get_left_matrix_row,
  char right_data_type,
  int right_matrix_width,
  int right_matrix_height,
  void* right_matrix,
  matrix_get_col get_right_matrix_col,
  matrix_constructor constructor
);