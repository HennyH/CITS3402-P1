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
#include "../CITS3402P1/log_file_writer.h"
#include "../CITS3402P1/cli_parser.h"

#include "spec_test_ad_float1024_float1024.c"
#include "spec_test_ad_float128_float128.c"
#include "spec_test_ad_float256_float256.c"
#include "spec_test_ad_float64_float64.c"
#include "spec_test_ad_int1024_int1024.c"
#include "spec_test_ad_int128_int128.c"
#include "spec_test_ad_int256_int256.c"
#include "spec_test_ad_int64_int64.c"
#include "spec_test_mm_float1024_float1024.c"
#include "spec_test_mm_float128_float128.c"
#include "spec_test_mm_float256_float256.c"
#include "spec_test_mm_float64_float64.c"
#include "spec_test_mm_int1024_int1024.c"
#include "spec_test_mm_int128_int128.c"
#include "spec_test_mm_int256_int256.c"
#include "spec_test_mm_int64_int64.c"
#include "spec_test_sm_float1024.c"
#include "spec_test_sm_float128.c"
#include "spec_test_sm_float256.c"
#include "spec_test_sm_float64.c"
#include "spec_test_sm_int1024.c"
#include "spec_test_sm_int128.c"
#include "spec_test_sm_int256.c"
#include "spec_test_sm_int64.c"
#include "spec_test_tr_float1024.c"
#include "spec_test_tr_float128.c"
#include "spec_test_tr_float256.c"
#include "spec_test_tr_float64.c"
#include "spec_test_tr_int1024.c"
#include "spec_test_tr_int128.c"
#include "spec_test_tr_int256.c"
#include "spec_test_tr_int64.c"
#include "spec_test_ts_float1024.c"
#include "spec_test_ts_float128.c"
#include "spec_test_ts_float256.c"
#include "spec_test_ts_float64.c"
#include "spec_test_ts_int1024.c"
#include "spec_test_ts_int128.c"
#include "spec_test_ts_int256.c"

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
  union matrix_value* values = read_file("./inputs/int_4x4.in", &data_type, &n_rows, &n_cols);
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
  union matrix_value* values = read_file("./inputs/float_4x4.in", &data_type, &n_rows, &n_cols);
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

START_TEST(matrix_op_mul_mixed_square_matrix)
{
  char result_data_type;
  char left_m_data_type, right_m_data_type;
  int left_m_n_cols, left_m_n_rows, right_m_n_cols, right_m_n_rows;
  void* left_m_values = read_file("./inputs/int_4x4.in", &left_m_data_type, &left_m_n_rows, &left_m_n_cols);
  void* right_m_values = read_file("./inputs/float_4x4.in", &right_m_data_type, &right_m_n_rows, &right_m_n_cols);
  struct csr_matrix* left_m = csr_matrix_constructor(left_m_data_type, left_m_n_cols, left_m_n_rows, left_m_values);
  struct csc_matrix* right_m = csc_matrix_constructor(right_m_data_type, right_m_n_cols, right_m_n_rows, right_m_values);
  struct coo_matrix* result;
  enum mop_errno_t error = matrix_multiply(
    left_m_data_type, left_m_n_cols, left_m_n_rows, left_m, &csr_matrix_get_row,
    right_m_data_type, right_m_n_cols, right_m_n_rows, right_m, &csc_matrix_get_col,
    &coo_matrix_constructor,
    &result,
    &result_data_type
  );
  ck_assert_int_eq(error, mop_errno_ok);
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

START_TEST(matrix_op_mul_int_rectangular)
{
  char result_data_type;
  char left_m_data_type, right_m_data_type;
  int left_m_n_cols, left_m_n_rows, right_m_n_cols, right_m_n_rows;
  void* left_m_values = read_file("./inputs/int_2x3.in", &left_m_data_type, &left_m_n_rows, &left_m_n_cols);
  void* right_m_values = read_file("./inputs/int_3x2.in", &right_m_data_type, &right_m_n_rows, &right_m_n_cols);
  struct csr_matrix* left_m = csr_matrix_constructor(left_m_data_type, left_m_n_cols, left_m_n_rows, left_m_values);
  struct csc_matrix* right_m = csc_matrix_constructor(right_m_data_type, right_m_n_cols, right_m_n_rows, right_m_values);
  struct coo_matrix* result;
  enum mop_errno_t error = matrix_multiply(
    left_m_data_type, left_m_n_cols, left_m_n_rows, left_m, &csr_matrix_get_row,
    right_m_data_type, right_m_n_cols, right_m_n_rows, right_m, &csc_matrix_get_col,
    &coo_matrix_constructor,
    &result,
    &result_data_type
  );
  ck_assert_int_eq(error, mop_errno_ok);
  /* the result of <identity> * <m> should just be <m> */
  ck_assert_int_eq(result->width, 2);
  ck_assert_int_eq(result->height, 2);
  ck_assert_int_eq(result->n_triples, 4);
  ck_assert_int_eq(result->data_type, DATA_TYPE_INTEGER);

  ck_assert_int_eq(coo_matrix_get_triple(result, 0).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(result, 0).row_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(result, 0).value.i, 15);

  ck_assert_int_eq(coo_matrix_get_triple(result, 1).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(result, 1).row_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(result, 1).value.i, 15);

  ck_assert_int_eq(coo_matrix_get_triple(result, 2).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(result, 2).row_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(result, 2).value.i, 7);

  ck_assert_int_eq(coo_matrix_get_triple(result, 3).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(result, 3).row_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(result, 3).value.i, 7);
}
END_TEST

START_TEST(matrix_op_mul_invalid_dimensions)
{
  char result_data_type;
  char left_m_data_type, right_m_data_type;
  int left_m_n_cols, left_m_n_rows, right_m_n_cols, right_m_n_rows;
  void* left_m_values = read_file("./inputs/int_4x4.in", &left_m_data_type, &left_m_n_rows, &left_m_n_cols);
  void* right_m_values = read_file("./inputs/int_3x2.in", &right_m_data_type, &right_m_n_rows, &right_m_n_cols);
  struct csr_matrix* left_m = csr_matrix_constructor(left_m_data_type, left_m_n_cols, left_m_n_rows, left_m_values);
  struct csc_matrix* right_m = csc_matrix_constructor(right_m_data_type, right_m_n_cols, right_m_n_rows, right_m_values);
  struct coo_matrix* result;
  enum mop_errno_t error = matrix_multiply(
    left_m_data_type, left_m_n_cols, left_m_n_rows, left_m, &csr_matrix_get_row,
    right_m_data_type, right_m_n_cols, right_m_n_rows, right_m, &csc_matrix_get_col,
    &coo_matrix_constructor,
    &result,
    &result_data_type
  );
  ck_assert_int_eq(error, mop_errno_dimension_incompatible);
}
END_TEST

START_TEST(matrix_op_add_mixed_square)
{
  char result_data_type;
  char left_m_data_type, right_m_data_type;
  int left_m_n_cols, left_m_n_rows, right_m_n_cols, right_m_n_rows;
  void* left_m_values = read_file("./inputs/int_4x4.in", &left_m_data_type, &left_m_n_rows, &left_m_n_cols);
  void* right_m_values = read_file("./inputs/float_4x4.in", &right_m_data_type, &right_m_n_rows, &right_m_n_cols);
  struct csc_matrix* left_m = csc_matrix_constructor(left_m_data_type, left_m_n_cols, left_m_n_rows, left_m_values);
  struct csc_matrix* right_m = csc_matrix_constructor(right_m_data_type, right_m_n_cols, right_m_n_rows, right_m_values);
  struct coo_matrix* result;
  enum mop_errno_t error = matrix_add(
    left_m_data_type, left_m_n_cols, left_m_n_rows, left_m, &csc_matrix_get_col,
    right_m_data_type, right_m_n_cols, right_m_n_rows, right_m, &csc_matrix_get_col,
    &coo_matrix_constructor,
    &result,
    &result_data_type
  );
  ck_assert_int_eq(error, mop_errno_ok);
  /* the result of <identity> * <m> should just be <m> */
  ck_assert_int_eq(result->width, 4);
  ck_assert_int_eq(result->height, 4);
  ck_assert_int_eq(result->n_triples, 5);
  ck_assert_int_eq(result->data_type, DATA_TYPE_FLOAT);

  ck_assert_int_eq(coo_matrix_get_triple(result, 0).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(result, 0).row_i, 0);
  ck_assert_float_eq(coo_matrix_get_triple(result, 0).value.f, 2.0);

  ck_assert_int_eq(coo_matrix_get_triple(result, 1).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(result, 1).row_i, 1);
  ck_assert_float_eq(coo_matrix_get_triple(result, 1).value.f, 1.0);

  ck_assert_int_eq(coo_matrix_get_triple(result, 2).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(result, 2).row_i, 2);
  ck_assert_float_eq(coo_matrix_get_triple(result, 2).value.f, 1.0);

  ck_assert_int_eq(coo_matrix_get_triple(result, 3).col_i, 2);
  ck_assert_int_eq(coo_matrix_get_triple(result, 3).row_i, 2);
  ck_assert_float_eq(coo_matrix_get_triple(result, 3).value.f, 1.5);

  ck_assert_int_eq(coo_matrix_get_triple(result, 4).col_i, 3);
  ck_assert_int_eq(coo_matrix_get_triple(result, 4).row_i, 3);
  ck_assert_float_eq(coo_matrix_get_triple(result, 4).value.f, 1.75);
}
END_TEST

START_TEST(matrix_op_transpose_int_square)
{
  char data_type;
  char result_data_type;
  int n_cols, n_rows;
  void* values = read_file("./inputs/int_2x2.in", &data_type, &n_rows, &n_cols);
  struct csc_matrix* m = csc_matrix_constructor(data_type, n_cols, n_rows, values);
  struct coo_matrix* m_transposed;
  enum mop_errno_t error = matrix_transpose(data_type, n_cols, n_rows, m, &csc_matrix_get_col, NULL, &coo_matrix_constructor, &m_transposed, &result_data_type);
  ck_assert_int_eq(error, mop_errno_ok);
  ck_assert_int_eq(m_transposed->width, 2);
  ck_assert_int_eq(m_transposed->height, 2);
  ck_assert_int_eq(m_transposed->n_triples, 4);
  ck_assert_int_eq(m_transposed->data_type, DATA_TYPE_INTEGER);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 0).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 0).row_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 0).value.i, 1);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 1).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 1).row_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 1).value.i, 2);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 2).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 2).row_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 2).value.i, 1);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 3).col_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 3).row_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 3).value.i, 2);
}
END_TEST

START_TEST(matrix_op_transpose_float_rectangluar)
{
  char result_data_type;
  char data_type;
  int n_cols, n_rows;
  void* values = read_file("./inputs/float_1x5.in", &data_type, &n_rows, &n_cols);
  struct csr_matrix* m = csr_matrix_constructor(data_type, n_cols, n_rows, values);
  struct coo_matrix* m_transposed;
  enum mop_errno_t error = matrix_transpose(data_type, n_cols, n_rows, m, NULL, &csr_matrix_get_row, &coo_matrix_constructor, &m_transposed, &result_data_type);
  ck_assert_int_eq(error, mop_errno_ok);
  ck_assert_int_eq(m_transposed->width, 1);
  ck_assert_int_eq(m_transposed->height, 5);
  ck_assert_int_eq(m_transposed->n_triples, 5);
  ck_assert_int_eq(m_transposed->data_type, DATA_TYPE_FLOAT);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 0).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 0).row_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 0).value.f, -1.0);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 1).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 1).row_i, 1);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 1).value.f, 2);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 2).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 2).row_i, 2);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 2).value.f, -3.0);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 3).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 3).row_i, 3);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 3).value.f, 4);

  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 4).col_i, 0);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 4).row_i, 4);
  ck_assert_int_eq(coo_matrix_get_triple(m_transposed, 4).value.f, -5.0);
}
END_TEST

START_TEST(cli_args_tr)
{
  char* operation = NULL;
  float sm_multiple = 2.0;
  char* input_file_1 = NULL;
  char* input_file_2 = NULL;
  int n_threads = 0;
  int log = 0;

  char* argv[] = { "./mop.exe", "--tr", "-f", "matrix1.in", "-l" };
  int argc = _countof(argv);
  char** envp = NULL;

  parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

  ck_assert_str_eq(operation, "tr");
  ck_assert_float_eq(sm_multiple, 2.0);
  ck_assert_str_eq(input_file_1, "matrix1.in");
  ck_assert_ptr_null(input_file_2);
  ck_assert_int_eq(n_threads, NULL);
  ck_assert_int_eq(log, 1);
}
END_TEST

START_TEST(cli_args_ts)
{
  char* operation = NULL;
  float sm_multiple = 2.0;
  char* input_file_1 = NULL;
  char* input_file_2 = NULL;
  int n_threads = 0;
  int log = 0;

  char* argv[] = { "./mop.exe", "--ts", "-f", "matrix1.in", "-t", "3" };
  int argc = _countof(argv);
  char** envp = NULL;

  parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

  ck_assert_str_eq(operation, "ts");
  ck_assert_float_eq(sm_multiple, 2.0);
  ck_assert_str_eq(input_file_1, "matrix1.in");
  ck_assert_ptr_null(input_file_2);
  ck_assert_int_eq(n_threads, 3);
  ck_assert_int_eq(log, 0);
}
END_TEST

START_TEST(cli_args_sm)
{
  char* operation = NULL;
  float sm_multiple = 2.0;
  char* input_file_1 = NULL;
  char* input_file_2 = NULL;
  int n_threads = 0;
  int log = 0;

  char* argv[] = { "./mop.exe", "--sm", "2.0", "-f", "matrix1.in" };
  int argc = _countof(argv);
  char** envp = NULL;

  parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

  ck_assert_str_eq(operation, "sm");
  ck_assert_float_eq(sm_multiple, 2.0);
  ck_assert_str_eq(input_file_1, "matrix1.in");
  ck_assert_ptr_null(input_file_2);
  ck_assert_int_eq(n_threads, NULL);
  ck_assert_int_eq(log, NULL);
}
END_TEST

START_TEST(cli_args_ad)
{
  char* operation = NULL;
  float sm_multiple = 2.0;
  char* input_file_1 = NULL;
  char* input_file_2 = NULL;
  int n_threads = 0;
  int log = 0;

  char* argv[] = { "./mop.exe", "--ad", "-f", "matrix1.in", "matrix2.in", "-l", "-t", "5" };
  int argc = _countof(argv);
  char** envp = NULL;

  parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

  ck_assert_str_eq(operation, "ad");
  ck_assert_float_eq(sm_multiple, 2.0);
  ck_assert_str_eq(input_file_1, "matrix1.in");
  ck_assert_str_eq(input_file_2, "matrix2.in");
  ck_assert_int_eq(n_threads, 5);
  ck_assert_int_eq(log, 1);
}
END_TEST

START_TEST(cli_args_mm)
{
  char* operation = NULL;
  float sm_multiple = 2.0;
  char* input_file_1 = NULL;
  char* input_file_2 = NULL;
  int n_threads = 0;
  int log = 0;

  char* argv[] = { "./mop.exe", "--mm", "-f", "matrix1.in", "./matrix2.in", "-t", "2" };
  int argc = _countof(argv);
  char** envp = NULL;

  parse_cli_args(argc, argv, envp, &operation, &sm_multiple, &input_file_1, &input_file_2, &n_threads, &log);

  ck_assert_str_eq(operation, "mm");
  ck_assert_float_eq(sm_multiple, 2.0);
  ck_assert_str_eq(input_file_1, "matrix1.in");
  ck_assert_str_eq(input_file_2, "./matrix2.in");
  ck_assert_int_eq(n_threads, 2);
  ck_assert_int_eq(log, 0);
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
  tcase_add_test(matrix_ops_test_case, matrix_op_mul_mixed_square_matrix);
  tcase_add_test(matrix_ops_test_case, matrix_op_mul_invalid_dimensions);
  tcase_add_test(matrix_ops_test_case, matrix_op_mul_int_rectangular);
  tcase_add_test(matrix_ops_test_case, matrix_op_add_mixed_square);
  tcase_add_test(matrix_ops_test_case, matrix_op_transpose_int_square);
  tcase_add_test(matrix_ops_test_case, matrix_op_transpose_float_rectangluar);

  TCase* cli_parser_test_case = tcase_create("CLI Parser");
  tcase_add_test(cli_parser_test_case, cli_args_sm);
  tcase_add_test(cli_parser_test_case, cli_args_tr);
  tcase_add_test(cli_parser_test_case, cli_args_ad);
  tcase_add_test(cli_parser_test_case, cli_args_ts);
  tcase_add_test(cli_parser_test_case, cli_args_mm);

  TCase* spec_test_case = tcase_create("Spec Tests");
  tcase_add_test(spec_test_case, spec_test_ad_float1024_float1024);
  tcase_add_test(spec_test_case, spec_test_ad_float128_float128);
  tcase_add_test(spec_test_case, spec_test_ad_float256_float256);
  tcase_add_test(spec_test_case, spec_test_ad_float64_float64);
  tcase_add_test(spec_test_case, spec_test_ad_int1024_int1024);
  tcase_add_test(spec_test_case, spec_test_ad_int128_int128);
  tcase_add_test(spec_test_case, spec_test_ad_int256_int256);
  tcase_add_test(spec_test_case, spec_test_ad_int64_int64);
  tcase_add_test(spec_test_case, spec_test_mm_float1024_float1024);
  tcase_add_test(spec_test_case, spec_test_mm_float128_float128);
  tcase_add_test(spec_test_case, spec_test_mm_float256_float256);
  tcase_add_test(spec_test_case, spec_test_mm_float64_float64);
  tcase_add_test(spec_test_case, spec_test_mm_int1024_int1024);
  tcase_add_test(spec_test_case, spec_test_mm_int128_int128);
  tcase_add_test(spec_test_case, spec_test_mm_int256_int256);
  tcase_add_test(spec_test_case, spec_test_mm_int64_int64);
  tcase_add_test(spec_test_case, spec_test_sm_float1024);
  tcase_add_test(spec_test_case, spec_test_sm_float128);
  tcase_add_test(spec_test_case, spec_test_sm_float256);
  tcase_add_test(spec_test_case, spec_test_sm_float64);
  tcase_add_test(spec_test_case, spec_test_sm_int1024);
  tcase_add_test(spec_test_case, spec_test_sm_int128);
  tcase_add_test(spec_test_case, spec_test_sm_int256);
  tcase_add_test(spec_test_case, spec_test_sm_int64);
  tcase_add_test(spec_test_case, spec_test_tr_float1024);
  tcase_add_test(spec_test_case, spec_test_tr_float128);
  tcase_add_test(spec_test_case, spec_test_tr_float256);
  tcase_add_test(spec_test_case, spec_test_tr_float64);
  tcase_add_test(spec_test_case, spec_test_tr_int1024);
  tcase_add_test(spec_test_case, spec_test_tr_int128);
  tcase_add_test(spec_test_case, spec_test_tr_int256);
  tcase_add_test(spec_test_case, spec_test_tr_int64);
  tcase_add_test(spec_test_case, spec_test_ts_float1024);
  tcase_add_test(spec_test_case, spec_test_ts_float128);
  tcase_add_test(spec_test_case, spec_test_ts_float256);
  tcase_add_test(spec_test_case, spec_test_ts_float64);
  tcase_add_test(spec_test_case, spec_test_ts_int1024);
  tcase_add_test(spec_test_case, spec_test_ts_int128);
  tcase_add_test(spec_test_case, spec_test_ts_int256);

  Suite* project_test_suite = suite_create("Project Test Suite");
  suite_add_tcase(project_test_suite, matrix_constructors_test_case);
  suite_add_tcase(project_test_suite, file_reader_test_case);
  suite_add_tcase(project_test_suite, matrix_ops_test_case);
  suite_add_tcase(project_test_suite, cli_parser_test_case);
  suite_add_tcase(project_test_suite, spec_test_case);

  SRunner* runner = srunner_create(project_test_suite);

  /* add extra test cases */
  srunner_run_all(runner, CK_VERBOSE);
  int n_failures = srunner_ntests_failed(runner);
  srunner_free(runner);

  return n_failures == 0 ? 0 : 1;
}