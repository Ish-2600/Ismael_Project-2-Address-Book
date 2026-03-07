#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include "ctype.h"
#include <string.h>
#include <unistd.h>

#include "unity.h"
#include "unity_internals.h"
#include "address_book.h"
#include "address_book_menu.h"

int get_option(int type, const char *msg);

void setUp(void) {
}

void tearDown(void) {
}

static int run_get_option_with_input(int type, const char *input)
{
  int pipefd[2];
  TEST_ASSERT_EQUAL_INT(0, pipe(pipefd));

  TEST_ASSERT_EQUAL_INT((int)strlen(input), (int)write(pipefd[1], input, strlen(input)));
  TEST_ASSERT_EQUAL_INT(0, close(pipefd[1]));

  int saved_stdin = dup(STDIN_FILENO);
  TEST_ASSERT_TRUE(saved_stdin >= 0);
  TEST_ASSERT_EQUAL_INT(0, dup2(pipefd[0], STDIN_FILENO));
  TEST_ASSERT_EQUAL_INT(0, close(pipefd[0]));

  int option = get_option(type, "");

  TEST_ASSERT_EQUAL_INT(0, dup2(saved_stdin, STDIN_FILENO));
  TEST_ASSERT_EQUAL_INT(0, close(saved_stdin));

  return option;
}

static int run_get_option_at_eof(int type)
{
  int pipefd[2];
  TEST_ASSERT_EQUAL_INT(0, pipe(pipefd));
  TEST_ASSERT_EQUAL_INT(0, close(pipefd[1]));

  int saved_stdin = dup(STDIN_FILENO);
  TEST_ASSERT_TRUE(saved_stdin >= 0);
  TEST_ASSERT_EQUAL_INT(0, dup2(pipefd[0], STDIN_FILENO));
  TEST_ASSERT_EQUAL_INT(0, close(pipefd[0]));

  int option = get_option(type, "");

  TEST_ASSERT_EQUAL_INT(0, dup2(saved_stdin, STDIN_FILENO));
  TEST_ASSERT_EQUAL_INT(0, close(saved_stdin));

  return option;
}

void test_get_option_for_CHAR(void) {
  int option = run_get_option_with_input(CHAR, "Y\n");
  TEST_ASSERT_EQUAL_INT('Y', option);
}

void test_get_option_for_CHAR_skips_whitespace_and_uppercases(void) {
  int option = run_get_option_with_input(CHAR, " \tq\n");
  TEST_ASSERT_EQUAL_INT('Q', option);
}

void test_get_option_for_NUM_returns_first_digit(void) {
  int option = run_get_option_with_input(NUM, "abc7def9\n");
  TEST_ASSERT_EQUAL_INT(7, option);
}

void test_get_option_for_NUM_returns_no_option_without_digits(void) {
  int option = run_get_option_with_input(NUM, "abc\n");
  TEST_ASSERT_EQUAL_INT(e_no_opt, option);
}

void test_get_option_for_NONE_returns_no_option(void) {
  int option = run_get_option_with_input(NONE, "ignored\n");
  TEST_ASSERT_EQUAL_INT(e_no_opt, option);
}

void test_get_option_returns_no_option_on_eof(void) {
  int option = run_get_option_at_eof(CHAR);
  TEST_ASSERT_EQUAL_INT(e_no_opt, option);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_get_option_for_CHAR);
  RUN_TEST(test_get_option_for_CHAR_skips_whitespace_and_uppercases);
  RUN_TEST(test_get_option_for_NUM_returns_first_digit);
  RUN_TEST(test_get_option_for_NUM_returns_no_option_without_digits);
  RUN_TEST(test_get_option_for_NONE_returns_no_option);
  RUN_TEST(test_get_option_returns_no_option_on_eof);

  return UNITY_END();
}
