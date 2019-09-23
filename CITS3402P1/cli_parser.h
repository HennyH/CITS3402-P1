#pragma once

#include "matrix.h"

void parse_cli_args(int argc, char* argv[], char** envp, char** operation, double* sm_multiple, char** input_file_1, char** input_file_2, int* n_threads, int* log);

enum mop_errno_t perform_cli_action(char* operation, double sm_multiple, char* input_file_1, char* input_file_2, int n_threads, double* load_seconds, double* operation_seconds, matrix_constructor constructor, void** matrix_result, union matrix_value* value_result, char* result_data_type);
