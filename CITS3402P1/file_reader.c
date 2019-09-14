#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
//void* read_file(char* filename, char** out_data_type, int* out_n_rows, int* out_n_cols) {
//  FILE* in_file = fopen(filename, "r");
//  *out_data_type = (char*)malloc(sizeof(char) * 5 /* float*/ + 1 /* null terminator */);
//  fscanf(in_file, "%s", *out_data_type);
//  fscanf(in_file, "%i", out_n_rows);
//  fscanf(in_file, "%i", out_n_cols);
//
//  int n_values = (*out_n_rows) * (*out_n_cols);
//  int* values = (int*)malloc(sizeof(int) * n_values);
//  for (int i = 0; i < n_values; i++) {
//    if ((*out_data_type)[0] == 'i') {
//      fscanf(in_file, "%d", values[i]);
//    }
//    else {
//      fscanf(in_file, "%f", values[i]);
//    }
//  }
//}
