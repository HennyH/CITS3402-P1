#pragma once

char* write_log_file(FILE* maybe_file, int value_limit, char* student_number, char* operation, char* input_filename_1, char* maybe_input_filename_2, int n_threads, clock_t load_time, clock_t calc_time, char data_type, int width, int height, union matrix_value value, void* matrix, matrix_get_row get_row);
