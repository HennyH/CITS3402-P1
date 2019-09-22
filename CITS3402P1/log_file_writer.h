#pragma once

char* log_file_write_matrix(char* student_number, char* operation, char* input_filename_1, char* maybe_input_filename_2, int n_threads, int ms_elapsed, char data_type, int width, int height, void* matrix, matrix_get_row get_row);