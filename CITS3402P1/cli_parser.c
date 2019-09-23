#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../CITS3402P1/matrix.h"
#include "../CITS3402P1/coo_matrix.h"
#include "../CITS3402P1/csc_matrix.h"
#include "../CITS3402P1/csr_matrix.h"
#include "../CITS3402P1/matrix_operations.h"
#include "../CITS3402P1/file_reader.h"
#include "../CITS3402P1/log_file_writer.h"
#include "../CITS3402P1/cli_parser.h"


void parse_cli_args(int argc, char* argv[], char** envp, char** operation, float* sm_multiple, char** input_file_1, char** input_file_2, int* n_threads, int* log)
{
  const int OPERARTION_SIZE = 2 + 1 /* null term */;

  for (int i = 0; i < argc; i++) {
    if (strcmp("--sm", argv[i]) == 0) {
      *operation = calloc(3, sizeof(char));
      strcpy_s(*operation, sizeof(char) * OPERARTION_SIZE, "sm");
      sscanf_s(argv[i + 1], "%f", sm_multiple);
    }
    else if (strcmp("--tr", argv[i]) == 0) {
      *operation = calloc(3, sizeof(char));
      strcpy_s(*operation, sizeof(char) * OPERARTION_SIZE, "tr");
    }
    else if (strcmp("--ad", argv[i]) == 0) {
      *operation = calloc(3, sizeof(char));
      strcpy_s(*operation, sizeof(char) * OPERARTION_SIZE, "ad");
    }
    else if (strcmp("--ts", argv[i]) == 0) {
      *operation = calloc(3, sizeof(char));
      strcpy_s(*operation, sizeof(char) * OPERARTION_SIZE, "ts");
    }
    else if (strcmp("--mm", argv[i]) == 0) {
      *operation = calloc(3, sizeof(char));
      strcpy_s(*operation, sizeof(char) * OPERARTION_SIZE, "mm");
    }
    else if (strcmp("-t", argv[i]) == 0) {
      sscanf_s(argv[i + 1], "%d", n_threads);
    }
    else if (strcmp("-l", argv[i]) == 0) {
      *log = 1;
    }
    else if (strcmp("-f", argv[i]) == 0 && operation != NULL) {
      if (strcmp("mm", *operation) == 0 || strcmp("ad", *operation) == 0) {
        *input_file_1 = calloc(1025, sizeof(char));
        *input_file_2 = calloc(1025, sizeof(char));
        sscanf_s(argv[i + 1], "%s", *input_file_1, 1024);
        sscanf_s(argv[i + 2], "%s", *input_file_2, 1024);
      }
      else {
        *input_file_1 = calloc(1025, sizeof(char));
        sscanf_s(argv[i + 1], "%s", *input_file_1, 1024);
      }
    }
  }
}

enum mop_errno_t perform_cli_action(char* operation, float sm_multiple, char* input_file_1, char* input_file_2, int n_threads, clock_t* load_ms, clock_t* operation_ms, matrix_constructor constructor, void** matrix_result, union matrix_value* value_result, char* result_data_type)
{
  enum mop_errno_t mop_errno = mop_no_such_op;
  clock_t load_start, load_finish, operation_finish;
  load_start = clock();
  load_finish = clock();

  if (strcmp("sm", operation) == 0) {
    char m_data_type;
    int m_rows, m_cols;
    union matrix_value* m_values = read_file(input_file_1, &m_data_type, &m_rows, &m_cols);
    struct csc_matrix* m = csc_matrix_constructor(m_data_type, m_cols, m_rows, m_values);
    load_finish = clock();
    mop_errno = matrix_scalar_multiply(m_data_type, sm_multiple, m, m_cols, m_rows, &csc_matrix_get_col, constructor, matrix_result, result_data_type);
    csc_matrix_free(&m);
  }
  else if (strcmp("tr", operation) == 0) {
    char m_data_type;
    int m_rows, m_cols;
    union matrix_value* m_values = read_file(input_file_1, &m_data_type, &m_rows, &m_cols);
    struct coo_matrix* m = coo_matrix_constructor(m_data_type, m_cols, m_rows, m_values);
    load_finish = clock();
    mop_errno = matrix_trace(m_data_type, m, value_result, result_data_type);
    coo_matrix_free(&m);
  }
  else if (strcmp("ts", operation) == 0) {
    char m_data_type;
    int m_rows, m_cols;
    union matrix_value* m_values = read_file(input_file_1, &m_data_type, &m_rows, &m_cols);
    struct csc_matrix* m = csc_matrix_constructor(m_data_type, m_cols, m_rows, m_values);
    load_finish = clock();
    mop_errno = matrix_transpose(m_data_type, m_cols, m_rows, m, &csc_matrix_get_col, NULL, constructor, matrix_result, result_data_type);
    csc_matrix_free(&m);
  }
  else {
    char left_m_data_type, right_m_data_type;
    int left_m_n_cols, left_m_n_rows, right_m_n_cols, right_m_n_rows;
    void* left_m_values = read_file(input_file_1, &left_m_data_type, &left_m_n_rows, &left_m_n_cols);
    void* right_m_values = read_file(input_file_2, &right_m_data_type, &right_m_n_rows, &right_m_n_cols);

    if (strcmp("ad", operation) == 0) {
      struct csc_matrix* left_m = csc_matrix_constructor(left_m_data_type, left_m_n_cols, left_m_n_rows, left_m_values);
      struct csc_matrix* right_m = csc_matrix_constructor(right_m_data_type, right_m_n_cols, right_m_n_rows, right_m_values);
      load_finish = clock();
      mop_errno = matrix_add(
        left_m_data_type, left_m_n_cols, left_m_n_rows, left_m, &csc_matrix_get_col,
        right_m_data_type, right_m_n_cols, right_m_n_rows, right_m, &csc_matrix_get_col,
        constructor,
        matrix_result,
        result_data_type
      );
      csc_matrix_free(&left_m);
      csc_matrix_free(&right_m);
    }
    else if (strcmp("mm", operation) == 0) {
      struct csr_matrix* left_m = csr_matrix_constructor(left_m_data_type, left_m_n_cols, left_m_n_rows, left_m_values);
      struct csc_matrix* right_m = csc_matrix_constructor(right_m_data_type, right_m_n_cols, right_m_n_rows, right_m_values);
      load_finish = clock();
      mop_errno = matrix_multiply(
        left_m_data_type, left_m_n_cols, left_m_n_rows, left_m, &csr_matrix_get_row,
        right_m_data_type, right_m_n_cols, right_m_n_rows, right_m, &csc_matrix_get_col,
        constructor,
        matrix_result,
        result_data_type
      );
      csr_matrix_free(&left_m);
      csc_matrix_free(&right_m);
    }
  }

  operation_finish = clock();

  *load_ms = (load_finish - load_start) / CLOCKS_PER_SEC * 1000;
  *operation_ms = (operation_finish - load_finish) / CLOCKS_PER_SEC * 1000;

  return mop_errno;
}