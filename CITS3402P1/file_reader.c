#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "matrix.h"

union matrix_value* read_file(char* filename, char* out_data_type, int* out_n_rows, int* out_n_cols) {
  FILE* in_file;
  errno_t open_err = fopen_s(&in_file, filename, "r");

  char data_type[10];
  fscanf_s(in_file, "%s", data_type, 10);
  *out_data_type = data_type[0];
  fscanf_s(in_file, "%i", out_n_rows);
  fscanf_s(in_file, "%i", out_n_cols);

  int n_values = (*out_n_rows) * (*out_n_cols);
  union matrix_value* values = malloc(sizeof(union matrix_value) * n_values);
  for (int i = 0; i < n_values; i++) {
    if (*out_data_type == DATA_TYPE_INTEGER) {
      fscanf_s(in_file, "%d", &(values[i].i));
    }
    else {
      fscanf_s(in_file, "%f", &(values[i].f));
    }
  }

  fclose(in_file);
  return values;
}


/* Based off https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer */
char* read_all(char* filename) {
  FILE* file;
  fopen_s(&file, filename, "r");
  fseek(file, 0, SEEK_END);
  long n_bytes = ftell(file);
  rewind(file);

  char* content = (char*)calloc(n_bytes, sizeof(char));
  fread(content, sizeof(char), n_bytes, file);
  fclose(file);

  content[n_bytes] = 0;

  return content;
}