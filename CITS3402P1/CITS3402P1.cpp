// CITS3402P1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "csc_matrix.h"

int main()
{
  int h = 4;
  int w = 7;
  int values[] = { 0,1,0,0,0,0,0,
                  0,0,2,1,0,0,1,
                  0,0,0,0,0,0,0,
                  0,0,0,1,0,3,0 };
  matrix_constructor cns = &csc_matrix_constructor;
  struct csc_matrix* m = (struct csc_matrix*)cns(w, h, values);
  int* col3 = csc_matrix_get_col(3, m);

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
