#pragma once

#include "matrix.h"

union matrix_value* read_file(char* filename, char* out_data_type, int* out_n_rows, int* out_n_cols);

char* read_all(char* filename);
