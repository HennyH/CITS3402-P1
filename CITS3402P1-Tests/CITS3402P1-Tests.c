// CITS3402P1-Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "check.h"

START_TEST(test_check)
{
  ck_assert_float_eq(1.0f, 1.0f);
}
END_TEST

int main(void)
{
  TCase* matrix_mult_test_case_square_mult = tcase_create("Square Matrices");
  tcase_add_test(matrix_mult_test_case_square_mult, test_check);

  Suite* matrix_mult_test_suite = suite_create("Matrix Multiplication");
  suite_add_tcase(matrix_mult_test_suite, matrix_mult_test_case_square_mult);

  SRunner* runner = srunner_create(matrix_mult_test_suite);

  /* add extra test cases */
  srunner_run_all(runner, CK_VERBOSE);
  int n_failures = srunner_ntests_failed(runner);
  srunner_free(runner);

  return n_failures == 0 ? 0 : 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
