#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"

enum mop_errno_t {
  mop_errno_ok,
  mop_errno_dimension_incompatible,
  mop_errno_expected_square,
  mop_errno_argument_invalid
};

enum mop_errno_t matrix_scalar_multiply(char data_type, float a, void* matrix, int width, int height, matrix_get_col get_col, matrix_constructor constructor, void** result_matrix);

enum mop_errno_t matrix_trace(char data_type, struct coo_matrix* coo_matrix, union matrix_value* trace);

enum mop_errno_t matrix_transpose(char data_type, int width, int height, void* matrix, matrix_get_col get_col, matrix_get_row get_row, matrix_constructor constructor, void** result_matrix);

enum mop_errno_t matrix_add(char left_data_type, int left_matrix_width, int left_matrix_height, void* left_matrix, matrix_get_col get_left_matrix_col, char right_data_type, int right_matrix_width, int right_matrix_height, void* right_matrix, matrix_get_col get_right_matrix_col, matrix_constructor constructor, void** result_matrix);

enum mop_errno_t matrix_multiply(char left_data_type, int left_matrix_width, int left_matrix_height, void* left_matrix, matrix_get_row get_left_matrix_row, char right_data_type, int right_matrix_width, int right_matrix_height, void* right_matrix, matrix_get_col get_right_matrix_col, matrix_constructor constructor, void** result_matrix);