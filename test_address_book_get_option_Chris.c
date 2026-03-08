#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "unity.h"
#include "address_book.h"
#include "address_book_menu.h"

/* get_option is implemented in address_book_menu.c but not declared in the header. */
int get_option(int type, const char *msg);

static int run_get_option_with_input(int type, const char *input)
{
	int saved_stdin_fd = dup(fileno(stdin));
	FILE *tmp = tmpfile();
	int result;

	TEST_ASSERT_NOT_EQUAL(-1, saved_stdin_fd);
	TEST_ASSERT_NOT_NULL(tmp);

	if ((input != NULL) && (*input != '\0'))
	{
		size_t len = strlen(input);
		TEST_ASSERT_EQUAL(len, fwrite(input, 1, len, tmp));
	}

	rewind(tmp);
	TEST_ASSERT_NOT_EQUAL(-1, dup2(fileno(tmp), fileno(stdin)));

	result = get_option(type, "");

	TEST_ASSERT_NOT_EQUAL(-1, dup2(saved_stdin_fd, fileno(stdin)));
	close(saved_stdin_fd);
	fclose(tmp);

	return result;
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_get_option_none_returns_no_opt_even_with_input(void)
{
	int option = run_get_option_with_input(NONE, "123\n");
	TEST_ASSERT_EQUAL_INT(e_no_opt, option);
}

void test_get_option_num_returns_first_digit_found(void)
{
	int option = run_get_option_with_input(NUM, "  abc7xyz9\n");
	TEST_ASSERT_EQUAL_INT(7, option);
}

void test_get_option_num_returns_no_opt_when_no_digit_exists(void)
{
	int option = run_get_option_with_input(NUM, "abc\n");
	TEST_ASSERT_EQUAL_INT(e_no_opt, option);
}

void test_get_option_char_returns_uppercase_first_non_space(void)
{
	int option = run_get_option_with_input(CHAR, "   y\n");
	TEST_ASSERT_EQUAL_INT('Y', option);
}

void test_get_option_char_returns_no_opt_for_whitespace_only(void)
{
	int option = run_get_option_with_input(CHAR, "   \t  \n");
	TEST_ASSERT_EQUAL_INT(e_no_opt, option);
}

void test_get_option_returns_no_opt_on_eof(void)
{
	int option = run_get_option_with_input(NUM, "");
	TEST_ASSERT_EQUAL_INT(e_no_opt, option);
}

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_get_option_none_returns_no_opt_even_with_input);
	RUN_TEST(test_get_option_num_returns_first_digit_found);
	RUN_TEST(test_get_option_num_returns_no_opt_when_no_digit_exists);
	RUN_TEST(test_get_option_char_returns_uppercase_first_non_space);
	RUN_TEST(test_get_option_char_returns_no_opt_for_whitespace_only);
	RUN_TEST(test_get_option_returns_no_opt_on_eof);

	return UNITY_END();
}
