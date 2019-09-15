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

START_TEST(coo_matrix_constructor_ints)
{
  int values[] = {
    1, 2,
    3, 0,
    0, 6
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
  float values[] = {
    1.0, 2.0,
    3.0, 0.0,
    0.0, 6.0
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
  int values[] = {
    0,1,0,0,0,0,0,
    0,0,2,1,0,0,1,
    0,0,0,0,0,0,0,
    0,0,0,1,0,3,0
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
  int values[] = {
    0,1,0,0,0,0,0,
    0,0,2,1,0,0,1,
    0,0,0,0,0,0,0,
    0,0,0,1,0,3,0
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

int main(void)
{
  TCase* matrix_constructors_test_case = tcase_create("Matrix Constructors");
  tcase_add_test(matrix_constructors_test_case, coo_matrix_constructor_ints);
  tcase_add_test(matrix_constructors_test_case, coo_matrix_constructor_floats);
  tcase_add_test(matrix_constructors_test_case, csr_matrix_constructor_ints);
  tcase_add_test(matrix_constructors_test_case, csc_matrix_constructor_ints);

  Suite* project_test_suite = suite_create("Project Test Suite");
  suite_add_tcase(project_test_suite, matrix_constructors_test_case);

  SRunner* runner = srunner_create(project_test_suite);

  /* add extra test cases */
  srunner_run_all(runner, CK_VERBOSE);
  int n_failures = srunner_ntests_failed(runner);
  srunner_free(runner);

  return n_failures == 0 ? 0 : 1;
}