import os
import sys
import traceback
import random
from operator import methodcaller
from itertools import chain
from collections import defaultdict
import math

import parmap

def get_args(filename):
    op_to_args = {
        "sm": ["input_file_1", "threads", "data_type", "rows", "columns", "values", "ttl", "ttf"],
        "tr": ["input_file_1", "threads", "values", "ttl", "ttf"],
        "ts": ["input_file_1", "threads", "data_type", "rows", "columns", "values", "ttl", "ttf"],
        "ad": ["input_file_1", "input_file_2", "threads", "data_type", "rows", "columns", "values", "ttl", "ttf"],
        "mm": ["input_file_1", "input_file_2", "threads", "data_type", "rows", "columns", "values", "ttl", "ttf"],
    }
    with open(filename, "r") as fileobj:
        operation = fileobj.readline().strip()
        argnames = op_to_args[operation]
        lines = map(methodcaller("strip"), fileobj.readlines())
        arg_dict = dict(zip(argnames, lines))
        arg_dict["operation"] = operation
        return arg_dict

def mk_assertions(rows, cols, expected_values, data_type):
    code = ""
    is_non_zero = lambda n: abs(0 - n) > 0.00001
    non_zeros = [(math.floor(i / cols), i % cols, v) for i, v in enumerate(expected_values) if is_non_zero(v)]
    non_zeros = [(triple_i, v) for triple_i, v in enumerate(non_zeros)]
    random.shuffle(non_zeros)
    print("making assertions for", data_type)
    for triple_i, (i, j, val) in non_zeros[:500]:
        code += "\n" + f"        ck_assert_int_eq(coo_matrix_get_triple(matrix_result, {triple_i}).col_i, {j});"
        code += "\n" + f"        ck_assert_int_eq(coo_matrix_get_triple(matrix_result, {triple_i}).row_i, {i});"
        if "int" in data_type:
            code += "\n" + f"        ck_assert_int_eq(coo_matrix_get_triple(matrix_result, {triple_i}).value.i, {val});"
        else:
            code += "\n" + f"        ck_assert_double_eq_tol(coo_matrix_get_triple(matrix_result, {triple_i}).value.d, {val}, 1e-6);"
    return code


def mk_test_case(name, operation, input_file_1, input_file_2, rows, columns, data_type, values):

    code = f"""
        char* operation = NULL;
        double sm_multiple = 2.0;
        char* input_file_1 = NULL;
        char* input_file_2 = NULL;
        int n_threads = 0;
        int log = 0;
    """
    code += f"""
        char* argv[] = {{ "./mop.exe", "--{operation}", "-f", "./inputs/{input_file_1}\""""
    if input_file_2:
        code += f", \"./inputs/{input_file_2}\" }};"
    else:
        code += f"}};"

    code += f"""
        int argc = _countof(argv);
        char** envp = NULL;

        parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

        struct coo_matrix* matrix_result;
        union matrix_value value_result;
        char result_data_type;
        double load_seconds, operation_seconds;
        enum mop_errno_t error = perform_cli_action(operation, sm_multiple, input_file_1, input_file_2, n_threads, &load_seconds, &operation_seconds, &coo_matrix_constructor, &matrix_result, &value_result, &result_data_type);
    """

    if rows is not None:
        assertions = mk_assertions(
            int(rows),
            int(columns),
            list(map(float if data_type == "float" else int, values.split())),
            data_type
        )
        assertions += """
         coo_matrix_free(&matrix_result);
        """
    elif "float" in input_file_1:
        assertions = f"""
        ck_assert_double_eq_tol(value_result.d, {float(values)}, 1e-6);
        """
    elif "int" in input_file_1:
        assertions = f"""
        ck_assert_int_eq(value_result.i, {int(values)});
        """
    return f"""
START_TEST({name})
    {{
        {code}

        {assertions}
    }}
END_TEST
    """

def mk_includes():
    return """
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

    """

def gen_test_file(op, arg):
    if arg.get("input_file_2") is not None:
        test_file = f"spec_test_{op}_{arg['input_file_1'].rstrip('.in')}_{arg['input_file_2'].rstrip('.in')}.c"
    else:
        test_file = f"spec_test_{op}_{arg['input_file_1'].rstrip('.in')}.c"
    print("generating", test_file, file=sys.stderr)
    if os.path.exists(test_file):
        print(test_file, "found so skipping gen", file=sys.stderr)
        return
    try:
        with open(test_file, "w") as tf:
            print(mk_includes(), file=tf)
            print(mk_test_case(test_file.rstrip(".c"),
                            op,
                            arg["input_file_1"],
                            arg.get("input_file_2"),
                            arg.get("rows"),
                            arg.get("columns"),
                            arg.get("data_type") if op != "sm" else "float",
                            arg["values"]),
                file=tf)
    except Exception as e:
        print("Failed to generate", test_file, "with error", traceback.format_exc())
        try:
            os.unlink(test_file)
        except:
            raise

if __name__ == "__main__":
    op_to_args = defaultdict(list)

    for dirpath, dirnames, filenames in os.walk("."):
        if dirpath == ".":
            continue
        operation = os.path.basename(dirpath)
        for i, output_filename in enumerate(filenames):
            if ".out" not in output_filename:
                continue
            output_filepath = os.path.join(dirpath, output_filename)
            args = get_args(output_filepath)
            op_to_args[operation].append(args)
    file_params = chain.from_iterable(
        [(op, arg) for arg in args]
        for op, args in op_to_args.items()
    )
    list(parmap.starmap(gen_test_file, file_params))
