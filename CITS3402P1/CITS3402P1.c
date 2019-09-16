// CITS3402P1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"
#include "csr_matrix.h"
#include "matrix_operations.h"


int main()
{


  float a[] = {
    1.0,1.0,
    2.0,2.0,
    3.0,3.0
  };
  float b[] = {
    1.0,
    2.0
  };
  struct csr_matrix* lm = csr_matrix_constructor('f', 2, 3, a);
  struct csc_matrix* rm = csc_matrix_constructor('f', 1, 2, b);
  struct coo_matrix* r;
  enum mop_errno_t error = matrix_multiply(
    'f', 2, 3, lm, &csr_matrix_get_row,
    'f', 1, 2, rm, &csc_matrix_get_col,
    &coo_matrix_constructor,
    &r
  );
  struct coo_triple* triples = r->triples;
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
