#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"

///<summary>Constructs a new matrix which is the result of a scalar multiplication</summary>
///<param name="a">The multiple by which to multiply <paramref name="matrix"/></param>
void* matrix_scalar_multiply(int a, void* matrix, int width, int height, matrix_get_col get_col, matrix_constructor constructor)
{
  int* result_values = (int*)malloc(sizeof(int) * width * height);

  for (int col_i = 0; col_i < width; col_i++)
  {
    int* column;
#pragma omp parallel shared(column, width, height, result_values)
    {
#pragma omp single
      column = get_col(col_i, matrix);
      int row_i = 0;
#pragma omp for
      for (row_i = 0; row_i < height; row_i++) {
        result_values[col_i * width + row_i] = a * column[row_i];
      }
    }
  }

  void* result_matrix = constructor(width, height, result_values);
  free(result_values);
  return result_matrix;
}

int matrix_trace(struct coo_matrix* coo_matrix)
{
  int i;
  int trace = 0;
  int n_values = coo_matrix->n_triples;
#pragma omp parallel for shared(coo_matrix) reduction(+: trace)
  for (i = 0; i < n_values; i++) {
    if (coo_matrix->triples[i].row_i == coo_matrix->triples[i].col_i) {
      trace += coo_matrix->triples[i].value;
    }
  }
  return trace;
}

void* matrix_add(int width, int height, void* left_matrix, void* right_matrix, matrix_get_col get_col, matrix_constructor constructor)
{
  int* result_values = (int*)malloc(sizeof(int) * width * height);
  for (int col_i = 0; col_i < width; col_i++)
  {
    int* left_column;
    int* right_column;
#pragma omp parallel shared(left_column, right_column, result_values)
    {
#pragma omp single
      {
        left_column = get_col(col_i, left_matrix);
        right_column = get_col(col_i, right_matrix);
      }
      int row_i = 0;
#pragma omp for
      for (row_i = 0; row_i < height; row_i++) {
        result_values[col_i * height + row_i] = left_column[row_i] + right_column[row_i];
      }
    }
  }

  void* result_matrix = constructor(width, height, result_values);
  free(result_values);
  return result_matrix;
}

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
) {
  const int result_matrix_width = right_matrix_width;
  const int result_matrix_height = left_matrix_height;
  int* result_values = (int*)malloc(sizeof(int) * result_matrix_width * result_matrix_height);
  int* left_matrix_row;

  for (int right_matrix_col_i = 0; right_matrix_col_i < right_matrix_width; right_matrix_col_i++) {
    int* right_matrix_column = get_right_matrix_col(right_matrix_col_i, right_matrix);
    int left_matrix_row_i;
#pragma omp parallel for shared(right_matrix_column, result_values) private(left_matrix_row)
    for (left_matrix_row_i = 0; left_matrix_row_i < left_matrix_height; left_matrix_row_i++) {
      left_matrix_row = get_left_matrix_row(left_matrix_row_i, left_matrix);
      int col_row_product_sum = 0;
      for (int i = 0; i < right_matrix_height; i++) {
        col_row_product_sum += right_matrix_column[i] * left_matrix_row[i];
      }
      result_values[right_matrix_col_i * result_matrix_width + left_matrix_row_i] = col_row_product_sum;
    }
  }

  void* result_matrix = constructor(result_matrix_width, result_matrix_height, result_values);
  free(result_values);
  return result_matrix;
}