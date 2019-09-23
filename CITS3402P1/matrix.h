#pragma once

#define DATA_TYPE_INTEGER 'i'
#define DATA_TYPE_DOUBLE 'f'

union matrix_value { int i; double d; };

inline int count_non_zero_values(char data_type, int width, int height, union matrix_value* values) {
  int n_non_zeros = 0;
  for (int row_i = 0; row_i < height; row_i++) {
    for (int col_i = 0; col_i < width; col_i++) {
      int i = col_i + row_i * width;
      if ((data_type == DATA_TYPE_INTEGER && values[i].i == 0) ||
        (data_type == DATA_TYPE_DOUBLE && values[i].d == 0.0)) {
        continue;
      }
      n_non_zeros++;
    }
  }

  return n_non_zeros;
}

inline int is_zero_value(char data_type, int i, union matrix_value* values) {
  return (data_type == DATA_TYPE_INTEGER && values[i].i == 0) ||
    (data_type == DATA_TYPE_DOUBLE && values[i].d == 0.0f);
}

inline void set_zero_matrix_value(char data_type, union matrix_value* value) {
  if (data_type == DATA_TYPE_INTEGER) {
    value->i = 0;
  }
  else {
    value->d = 0.0;
  }
}

inline void set_ltr_ttb_value(int row_i, int col_i, int width, int height, union matrix_value value, union matrix_value* values) {
  values[row_i * width + col_i] = value;
}

typedef void* (*matrix_constructor)(char data_type, int width, int height, union matrix_value* values);
typedef int (*matrix_get_width)(void* matrix);
typedef int (*matrix_get_height)(void* matrix);
typedef union matrix_value* (*matrix_get_row)(int row_i, void* matrix);
typedef union matrix_value* (*matrix_get_col)(int col_i, void* matrix);
