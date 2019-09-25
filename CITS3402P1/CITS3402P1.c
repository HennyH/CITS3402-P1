// CITS3402P1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "matrix.h"
#include "coo_matrix.h"
#include "csc_matrix.h"
#include "csr_matrix.h"
#include "matrix_operations.h"
#include "cli_parser.h"
#include "log_file_writer.h"


int main(int argc, char *argv[], char** envp)
{
  char* operation = NULL;
  double sm_multiple = 2.0;
  char* input_file_1 = NULL;
  char* input_file_2 = NULL;
  int n_threads = 0;
  int log = 0;

  parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

  struct csr_matrix* matrix_result = NULL;
  union matrix_value value_result;
  char result_data_type;
  clock_t load_ms, operation_ms;
  enum mop_errno_t error = perform_cli_action(operation, sm_multiple, input_file_1, input_file_2, n_threads, &load_ms, &operation_ms, &csr_matrix_constructor, &matrix_result, &value_result, &result_data_type);

  if (error != mop_errno_ok) {
    printf("Error performing operation (error no: %d)", error);
    exit(1);
  }

  char* filename = write_log_file(
    log ? NULL : stdout,
    log ? 0 : 50,
    "21471423",
    operation,
    input_file_1,
    input_file_2,
    n_threads,
    load_ms,
    operation_ms,
    result_data_type,
    matrix_result == NULL ? -1 : matrix_result->width,
    matrix_result == NULL ? -1 : matrix_result->height,
    value_result,
    matrix_result,
    &csr_matrix_get_row
  );

  if (log != NULL) {
    printf("Wrote log file %s", filename);
  }
 
  free(filename);
  if (matrix_result != NULL) {
    csr_matrix_free(&matrix_result);
  }
  free(operation);
  free(input_file_1);
  if (input_file_2 != NULL) {
    free(input_file_2);
  }

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
