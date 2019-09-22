#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <time.h>
#include "matrix.h"
#include "log_file_writer.h"

/* The log file should be in the following format:
 * op
 * in_file_1
 * [in_file_2]
 * n_threads
 * [matrix_type]
 * [matrix_width]
 * [matrix_height]
 * result
 */

char* log_file_get_filename(char* student_number, char* operation, int ms_elapsed) {
  time_t current_time = time(NULL);
  struct tm current_local_time;
  localtime_s(&current_local_time, &current_time);
  char timestamp[10];
  strftime(timestamp, _countof(timestamp) * sizeof(char), "%d%m%y", &current_local_time);

  /* filename is of the from <student_number>_<timestamp>_<execution_time>_<op>.out */
  const int FILNAME_LEN = 40;
  char* filename = (char*)calloc(FILNAME_LEN, sizeof(char));
  sprintf_s(filename, FILNAME_LEN, "%s_%s_%04d_%s.out", student_number, timestamp, ms_elapsed, operation);

  return filename;
}

void log_file_write_header(FILE* log_file, char* operation, char* input_filename_1, char* maybe_input_filename_2, int n_threads) {
  fprintf_s(log_file, "%s\n", operation);
  fprintf_s(log_file, "%s\n", input_filename_1);
  if (maybe_input_filename_2 != NULL) {
    fprintf_s(log_file, "%s\n", maybe_input_filename_2);
  }
  fprintf_s(log_file, "%d\n", n_threads);
}

char* log_file_write_matrix(char* student_number, char* operation, char* input_filename_1, char* maybe_input_filename_2, int n_threads, int ms_elapsed, char data_type, int width, int height, void* matrix, matrix_get_row get_row) {
  char* filename = log_file_get_filename(student_number, operation, ms_elapsed);

  FILE* log_file;
  errno_t open_err = fopen_s(&log_file, filename, "w+");

  log_file_write_header(log_file, operation, input_filename_1, maybe_input_filename_2, n_threads);
  fprintf_s(log_file, "%s\n", data_type == DATA_TYPE_INTEGER ? "int" : "float");
  fprintf_s(log_file, "%d\n", width);
  fprintf_s(log_file, "%d\n", height);

  for (int row_i = 0; row_i < height; row_i++) {
    union matrix_value* row_values = get_row(row_i, matrix);
    for (int col_i = 0; col_i < width; col_i++) {
      union matrix_value value = row_values[col_i];
      if (data_type == DATA_TYPE_INTEGER) {
        fprintf_s(log_file, "%d", value.i);
      }
      else {
        fprintf_s(log_file, "%f", value.f);
      }

      int i = row_i * width + col_i;
      if (i == width * height - 1) {
        fprintf_s(log_file, "\n");
      }
      else {
        fprintf_s(log_file, " ");
      }
    }
  }

  fclose(log_file);
  return filename;
}