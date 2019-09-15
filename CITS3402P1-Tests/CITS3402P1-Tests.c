// CITS3402P1-Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

START_TEST(coo_matrix_constructor_ints)
{
  union matrix_value values[] = {
    {.i = 1}, {.i = 2},
    {.i = 3}, {.i = 0},
    {.i = 0}, {.i = 6}
  };
  struct coo_matrix* m = coo_matrix_constructor(DATA_TYPE_INTEGER, 2, 3, values);
  ck_assert_int_eq(m->width, 2);
  ck_assert_int_eq(m->height, 3);
  ck_assert_int_eq(m->n_triples, 4);
  ck_assert_int_eq(m->data_type, DATA_TYPE_INTEGER);
  /* the COO format should store values left->right, top->bottom */
  /* check (0, 0, 1) is first */
  ck_assert_int_eq(coo_matrix_get_triple(m, 0).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m, 0).row_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m, 0).value.i, 1);
  /* check (1, 0, 2) is second */
  ck_assert_int_eq(coo_matrix_get_triple(m, 1).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m, 1).row_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m, 1).value.i, 2);
  /* check (0, 1, 3) is third */
  ck_assert_int_eq(coo_matrix_get_triple(m, 2).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m, 2).row_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m, 2).value.i, 3);
  /* check (1, 2, 6) is fourth */
  ck_assert_int_eq(coo_matrix_get_triple(m, 3).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m, 3).row_i, 2);
  ck_assert_int_eq(coo_matrix_get_triple(m, 3).value.i, 6);
}
END_TEST

START_TEST(coo_matrix_constructor_floats)
{
  union matrix_value values[] = {
    {.f = 1.0}, {.f = 2.0},
    {.f = 3.0}, {.f = 0.0},
    {.f = 0.0}, {.f = 6.0}
  };
  struct coo_matrix* m = coo_matrix_constructor(DATA_TYPE_FLOAT, 2, 3, values);
  ck_assert_int_eq(m->width, 2);
  ck_assert_int_eq(m->height, 3);
  ck_assert_int_eq(m->n_triples, 4);
  ck_assert_int_eq(m->data_type, DATA_TYPE_FLOAT);
  /* the COO format should store values left->right, top->bottom */
  /* check (0, 0, 1.0) is first */
  ck_assert_int_eq(coo_matrix_get_triple(m, 0).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m, 0).row_i, 0);
  ck_assert_float_eq(coo_matrix_get_triple(m, 0).value.f, 1.0);
  /* check (1, 0, 2.0) is second */
  ck_assert_int_eq(coo_matrix_get_triple(m, 1).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m, 1).row_i, 0);
  ck_assert_float_eq(coo_matrix_get_triple(m, 1).value.f, 2.0);
  /* check (0, 1, 3.0) is third */
  ck_assert_int_eq(coo_matrix_get_triple(m, 2).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m, 2).row_i, 1);
  ck_assert_float_eq(coo_matrix_get_triple(m, 2).value.f, 3.0);
  /* check (1, 2, 6.0) is fourth */
  ck_assert_int_eq(coo_matrix_get_triple(m, 3).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m, 3).row_i, 2);
  ck_assert_float_eq(coo_matrix_get_triple(m, 3).value.f, 6.0);
}
END_TEST

START_TEST(csr_matrix_constructor_ints)
{
  union matrix_value values[] = {
    {.i = 0},{.i = 1},{.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},
    {.i = 0},{.i = 0},{.i = 2},{.i = 1},{.i = 0},{.i = 0},{.i = 1},
    {.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},
    {.i = 0},{.i = 0},{.i = 0},{.i = 1},{.i = 0},{.i = 3},{.i = 0}
  };
  struct csr_matrix* m = csr_matrix_constructor(DATA_TYPE_INTEGER, 7, 4, values);
  ck_assert_int_eq(m->width, 7);
  ck_assert_int_eq(m->height, 4);
  ck_assert_int_eq(m->n_non_zeros, 6);
  ck_assert_int_eq(m->cnzs_len, 5);
  ck_assert_int_eq(m->col_is_len, 6);
  ck_assert_int_eq(m->vals_len, 6);
  ck_assert_int_eq(m->data_type, DATA_TYPE_INTEGER);
  /* check cnzs = [0, 1, 4, 4, 6] */
  ck_assert_int_eq(m->cnzs[0], 0);
  ck_assert_int_eq(m->cnzs[1], 1);
  ck_assert_int_eq(m->cnzs[2], 4);
  ck_assert_int_eq(m->cnzs[3], 4);
  ck_assert_int_eq(m->cnzs[4], 6);
  /* check columns = [1, 2, 3, 6, 3, 5] (these are indicies) */
  ck_assert_int_eq(m->col_is[0], 1);
  ck_assert_int_eq(m->col_is[1], 2);
  ck_assert_int_eq(m->col_is[2], 3);
  ck_assert_int_eq(m->col_is[3], 6);
  ck_assert_int_eq(m->col_is[4], 3);
  ck_assert_int_eq(m->col_is[5], 5);
  /* check vals = [1, 2, 1, 1, 1, 3] */
  ck_assert_int_eq(m->vals[0].i, 1);
  ck_assert_int_eq(m->vals[1].i, 2);
  ck_assert_int_eq(m->vals[2].i, 1);
  ck_assert_int_eq(m->vals[3].i, 1);
  ck_assert_int_eq(m->vals[4].i, 1);
  ck_assert_int_eq(m->vals[5].i, 3);
}
END_TEST

START_TEST(csc_matrix_constructor_ints)
{
  union matrix_value values[] = {
    {.i = 0},{.i = 1},{.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},
    {.i = 0},{.i = 0},{.i = 2},{.i = 1},{.i = 0},{.i = 0},{.i = 1},
    {.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},{.i = 0},
    {.i = 0},{.i = 0},{.i = 0},{.i = 1},{.i = 0},{.i = 3},{.i = 0}
  };
  struct csc_matrix* m = csc_matrix_constructor(DATA_TYPE_INTEGER, 7, 4, values);
  ck_assert_int_eq(m->width, 7);
  ck_assert_int_eq(m->height, 4);
  ck_assert_int_eq(m->n_non_zeros, 6);
  ck_assert_int_eq(m->cnzs_len, 8);
  ck_assert_int_eq(m->row_is_len, 6);
  ck_assert_int_eq(m->vals_len, 6);
  ck_assert_int_eq(m->data_type, DATA_TYPE_INTEGER);
  /* check cnzs = [0, 0, 1, 2, 4, 4, 5, 6] */
  ck_assert_int_eq(m->cnzs[0], 0);
  ck_assert_int_eq(m->cnzs[1], 0);
  ck_assert_int_eq(m->cnzs[2], 1);
  ck_assert_int_eq(m->cnzs[3], 2);
  ck_assert_int_eq(m->cnzs[4], 4);
  ck_assert_int_eq(m->cnzs[5], 4);
  ck_assert_int_eq(m->cnzs[6], 5);
  ck_assert_int_eq(m->cnzs[7], 6);
  /* check rows = [0, 1, 1, 3, 3, 1] (these are indicies) */
  ck_assert_int_eq(m->row_is[0], 0);
  ck_assert_int_eq(m->row_is[1], 1);
  ck_assert_int_eq(m->row_is[2], 1);
  ck_assert_int_eq(m->row_is[3], 3);
  ck_assert_int_eq(m->row_is[4], 3);
  ck_assert_int_eq(m->row_is[5], 1);
  /* check vals = [1, 2, 1, 1, 3, 1] */
  ck_assert_int_eq(m->vals[0].i, 1);
  ck_assert_int_eq(m->vals[1].i, 2);
  ck_assert_int_eq(m->vals[2].i, 1);
  ck_assert_int_eq(m->vals[3].i, 1);
  ck_assert_int_eq(m->vals[4].i, 3);
  ck_assert_int_eq(m->vals[5].i, 1);
}
END_TEST

START_TEST(file_reader_reads_int1)
{
  char data_type;
  int n_rows, n_cols;
  union matrix_value* values = read_file("int1.in", &data_type, &n_rows, &n_cols);
  ck_assert_int_eq(data_type, DATA_TYPE_INTEGER);
  ck_assert_int_eq(n_rows, 4);
  ck_assert_int_eq(n_cols, 4);
  ck_assert_int_eq(values[0].i, 1);
  ck_assert_int_eq(values[1].i, 0);
  ck_assert_int_eq(values[2].i, 0);
  ck_assert_int_eq(values[3].i, 0);
  ck_assert_int_eq(values[4].i, 0);
  ck_assert_int_eq(values[5].i, 1);
  ck_assert_int_eq(values[6].i, 0);
  ck_assert_int_eq(values[7].i, 0);
  ck_assert_int_eq(values[8].i, 0);
  ck_assert_int_eq(values[9].i, 0);
  ck_assert_int_eq(values[10].i, 1);
  ck_assert_int_eq(values[11].i, 0);
  ck_assert_int_eq(values[12].i, 0);
  ck_assert_int_eq(values[13].i, 0);
  ck_assert_int_eq(values[14].i, 0);
  ck_assert_int_eq(values[15].i, 1);
}
END_TEST

START_TEST(file_reader_reads_float1)
{
  char data_type;
  int n_rows, n_cols;
  union matrix_value* values = read_file("float1.in", &data_type, &n_rows, &n_cols);
  ck_assert_int_eq(data_type, DATA_TYPE_FLOAT);
  ck_assert_int_eq(n_rows, 4);
  ck_assert_int_eq(n_cols, 4);
  ck_assert_float_eq(values[0].f, 1.0);
  ck_assert_float_eq(values[1].f, 0.0);
  ck_assert_float_eq(values[2].f, 0.0);
  ck_assert_float_eq(values[3].f, 0.0);
  ck_assert_float_eq(values[4].f, 0.0);
  ck_assert_float_eq(values[5].f, 0.0);
  ck_assert_float_eq(values[6].f, 0.0);
  ck_assert_float_eq(values[7].f, 0.0);
  ck_assert_float_eq(values[8].f, 0.0);
  ck_assert_float_eq(values[9].f, 1.0);
  ck_assert_float_eq(values[10].f, 0.5);
  ck_assert_float_eq(values[11].f, 0.0);
  ck_assert_float_eq(values[12].f, 0.0);
  ck_assert_float_eq(values[13].f, 0.0);
  ck_assert_float_eq(values[14].f, 0.0);
  ck_assert_float_eq(values[15].f, 0.75);
}
END_TEST

START_TEST(matrix_op_mul_int_m_and_float_n)
{
  char left_m_data_type, right_m_data_type;
  int left_m_n_cols, left_m_n_rows, right_m_n_cols, right_m_n_rows;
  void* left_m_values = read_file("int1.in", &left_m_data_type, &left_m_n_rows, &left_m_n_cols);
  void* right_m_values = read_file("float1.in", &right_m_data_type, &right_m_n_rows, &right_m_n_cols);
  struct csr_matrix* left_m = csr_matrix_constructor(left_m_data_type, left_m_n_cols, left_m_n_rows, left_m_values);
  struct csc_matrix* right_m = csc_matrix_constructor(right_m_data_type, right_m_n_cols, right_m_n_rows, right_m_values);
  struct coo_matrix* result = matrix_multiply(
    left_m_data_type, left_m_n_cols, left_m_n_rows, left_m, &csr_matrix_get_row,
    right_m_data_type, right_m_n_cols, right_m_n_rows, right_m, &csc_matrix_get_col,
    &coo_matrix_constructor
  );
  /* the result of <identity> * <m> should just be <m> */
  ck_assert_int_eq(result->width, 4);
  ck_assert_int_eq(result->height, 4);
  ck_assert_int_eq(result->n_triples, 4);
  ck_assert_int_eq(result->data_type, DATA_TYPE_FLOAT);

  ck_assert_int_eq(coo_matrix_get_triple(result, 0).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(result, 0).row_i, 0);
  ck_assert_float_eq(coo_matrix_get_triple(result, 0).value.f, 1.0);

  ck_assert_int_eq(coo_matrix_get_triple(result, 1).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(result, 1).row_i, 2);
  ck_assert_float_eq(coo_matrix_get_triple(result, 1).value.f, 1.0);
  
  ck_assert_int_eq(coo_matrix_get_triple(result, 2).col_i, 2);
  ck_assert_int_eq(coo_matrix_get_triple(result, 2).row_i, 2);
  ck_assert_float_eq(coo_matrix_get_triple(result, 2).value.f, 0.5);
  
  ck_assert_int_eq(coo_matrix_get_triple(result, 3).col_i, 3);
  ck_assert_int_eq(coo_matrix_get_triple(result, 3).row_i, 3);
  ck_assert_float_eq(coo_matrix_get_triple(result, 3).value.f, 0.75);
}
END_TEST

int main(void)
{
  TCase* matrix_constructors_test_case = tcase_create("Matrix Constructors");
  tcase_add_test(matrix_constructors_test_case, coo_matrix_constructor_ints);
  tcase_add_test(matrix_constructors_test_case, coo_matrix_constructor_floats);
  tcase_add_test(matrix_constructors_test_case, csr_matrix_constructor_ints);
  tcase_add_test(matrix_constructors_test_case, csc_matrix_constructor_ints);

  TCase* file_reader_test_case = tcase_create("File Reader");
  tcase_add_test(file_reader_test_case, file_reader_reads_int1);
  tcase_add_test(file_reader_test_case, file_reader_reads_float1);

  TCase* matrix_ops_test_case = tcase_create("Matrix Operations");
  tcase_add_test(matrix_ops_test_case, matrix_op_mul_int_m_and_float_n);

  Suite* project_test_suite = suite_create("Project Test Suite");
  suite_add_tcase(project_test_suite, matrix_constructors_test_case);
  suite_add_tcase(project_test_suite, file_reader_test_case);
  suite_add_tcase(project_test_suite, matrix_ops_test_case);

  SRunner* runner = srunner_create(project_test_suite);

  /* add extra test cases */
  srunner_run_all(runner, CK_VERBOSE);
  int n_failures = srunner_ntests_failed(runner);
  srunner_free(runner);

  return n_failures == 0 ? 0 : 1;
}