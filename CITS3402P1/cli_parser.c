#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli_parser.h"

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
