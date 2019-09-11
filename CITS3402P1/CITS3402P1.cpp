// CITS3402P1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "csc_matrix.h"

///<summary>Constructs a new matrix which is the result of a scalar multiplication</summary>
///<param name="a">The multiple by which to multiply <paramref name="matrix"/></param>
void* matrix_scalar_multiply(int a, void* matrix, int width, int height, matrix_get_col get_col, matrix_constructor constructor)
{
  int* result_values = (int*)malloc(sizeof(int) * width * height);

  for (int col_i = 0; col_i < width; col_i++)
  {
    int* column;
#pragma omp parallel shared(column, width, height)
    {
#pragma omp single
      column = get_col(col_i + 1, matrix);
      int row_i = 0;
#pragma omp for
      for (row_i = 0; row_i < height; row_i++) {
        result_values[col_i * width + row_i] = a * column[row_i];
      }
    }
  }

  return constructor(width, height, result_values);
}

int main()
{
  /*int h = 4;
  int w = 7;
  int values[] = { 0,1,0,0,0,0,0,
                  0,0,2,1,0,0,1,
                  0,0,0,0,0,0,0,
                  0,0,0,1,0,3,0 };
  matrix_constructor cns = &csc_matrix_constructor;
  struct csc_matrix* m = (struct csc_matrix*)cns(w, h, values);
  int* col3 = csc_matrix_get_col(3, m);*/

  int i[] = { 1,1,1,1 };
  struct csc_matrix* i2 = (struct csc_matrix*)csc_matrix_constructor(2, 2, i);
  struct csc_matrix* r = (struct csc_matrix*)matrix_scalar_multiply(5, i2, 2, 2, &csc_matrix_get_col, &csc_matrix_constructor);
  printf("Hello world");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
