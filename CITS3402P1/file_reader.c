#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "matrix.h"

void* read_file(char* filename, char* out_data_type, int* out_n_rows, int* out_n_cols) {
  char cwd[200];
  _getcwd(cwd, sizeof(char) * 200);
  FILE* in_file;
  errno_t open_err = fopen_s(&in_file, filename, "r");

  char data_type[10];
  fscanf_s(in_file, "%s", data_type, 10);
  *out_data_type = data_type[0];
  fscanf_s(in_file, "%i", out_n_rows);
  fscanf_s(in_file, "%i", out_n_cols);

  int n_values = (*out_n_rows) * (*out_n_cols);
  void* values = *out_data_type == DATA_TYPE_INTEGER
    ? malloc(sizeof(int) * n_values)
    : malloc(sizeof(float) * n_values);
  for (int i = 0; i < n_values; i++) {
    if (*out_data_type == DATA_TYPE_INTEGER) {
      fscanf_s(in_file, "%d", &((int*)values)[i]);
    }
    else {
      fscanf_s(in_file, "%f", &((float*)values)[i]);
    }
  }

  return values;
}
