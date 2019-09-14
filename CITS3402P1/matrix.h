#pragma once

#define DATA_TYPE_INTEGER 'i'
#define DATA_TYPE_FLOAT 'f'

typedef union matrix_value { int i; float f; };

inline int count_non_zero_values(char data_type, int width, int height, void* values) {
  int n_non_zeros = 0;
  for (int row_i = 0; row_i < height; row_i++) {
    for (int col_i = 0; col_i < width; col_i++) {
      int i = col_i + row_i * width;
      if ((data_type == DATA_TYPE_INTEGER && ((int*)values)[i] == 0) ||
        (data_type == DATA_TYPE_FLOAT && ((float*)values)[i] == 0.0f)) {
        continue;
      }
      n_non_zeros++;
    }
  }

  return n_non_zeros;
}

inline int is_zero_value(char data_type, int i, void* values) {
  return (data_type == DATA_TYPE_INTEGER && ((int*)values)[i] == 0) ||
    (data_type == DATA_TYPE_FLOAT && ((float*)values)[i] == 0.0f);
}

inline void set_zero_matrix_value(char data_type, union matrix_value* value) {
  if (data_type == DATA_TYPE_INTEGER) {
    value->i = 0;
  }
  else {
    value->f = 0.0f;
  }
}

typedef void* (*matrix_constructor)(char data_type, int width, int height, int* values);
typedef int (*matrix_get_width)(void* matrix);
typedef int (*matrix_get_height)(void* matrix);
typedef union matrix_value* (*matrix_get_row)(int row_i, void* matrix);
typedef union matrix_value* (*matrix_get_col)(int col_i, void* matrix);
