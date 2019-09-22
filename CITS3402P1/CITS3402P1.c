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
#include "cli_parser.h"


int main(int argc, char *argv[], char** envp)
{
  char* operation = calloc(3, sizeof(char));
  float sm_multiple;
  char* input_file_1 = calloc(1025, sizeof(char));
  char* input_file_2 = calloc(1025, sizeof(char));
  int n_threads;
  int log;
  parse_cli_args(argc, argv, envp, operation, &sm_multiple, input_file_1, input_file_2, &n_threads, &log);
  return 0;
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
