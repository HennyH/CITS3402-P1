#pragma once

struct matrix
{
  int width;
  int height;
  void* data;
};

typedef void* (*matrix_constructor)(int width, int height, int* values);
typedef int (*matrix_get_width)(void* matrix);
typedef int (*matrix_get_height)(void* matrix);
typedef int* (*matrix_get_row)(int row, void* matrix);
typedef int* (*matrix_get_col)(int col, void* matrix);
