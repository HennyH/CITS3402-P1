
    #include <stdint.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "check.h"
    #include "../CITS3402P1/matrix.h"
    #include "../CITS3402P1/coo_matrix.h"
    #include "../CITS3402P1/csc_matrix.h"
    #include "../CITS3402P1/csr_matrix.h"
    #include "../CITS3402P1/matrix_operations.h"
    #include "../CITS3402P1/file_reader.h"
    #include "../CITS3402P1/log_file_writer.h"
    #include "../CITS3402P1/cli_parser.h"

    

START_TEST(spec_test_tr_float64)
    {
        
        char* operation = NULL;
        double sm_multiple = 2.0;
        char* input_file_1 = NULL;
        char* input_file_2 = NULL;
        int n_threads = 0;
        int log = 0;
    
        char* argv[] = { "./mop.exe", "--tr", "-f", "./inputs/float64.in"};
        int argc = _countof(argv);
        char** envp = NULL;

        parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

        struct coo_matrix* matrix_result;
        union matrix_value value_result;
        char result_data_type;
        double load_seconds, operation_seconds;
        enum mop_errno_t error = perform_cli_action(operation, sm_multiple, input_file_1, input_file_2, n_threads, &load_seconds, &operation_seconds, &coo_matrix_constructor, &matrix_result, &value_result, &result_data_type);
    

        
        ck_assert_double_eq_tol(value_result.d, 2950.104851, 1e-6);
        
    }
END_TEST
    
