#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "unity.h"
#include "address_book.h"
#include "address_book_menu.h"

/* ---------- helpers ---------- */

static AddressBook make_contacts(int count)
{
	AddressBook book;
	book.fp    = NULL;
	book.count = count;
	book.list  = calloc(count, sizeof(ContactInfo));

	for (int i = 0; i < count; i++) {
		snprintf(book.list[i].name[0], NAME_LEN, "Contact %d", i + 1);
		book.list[i].si_no = i + 1;
	}

	return book;
}

static Status run_list_contacts(AddressBook *book, int *index, const char *input)
{
	int saved_stdin_fd = dup(fileno(stdin));
	FILE *tmp = tmpfile();
	Status result;

	TEST_ASSERT_NOT_EQUAL(-1, saved_stdin_fd);
	TEST_ASSERT_NOT_NULL(tmp);

	if (input != NULL && *input != '\0') {
		size_t len = strlen(input);
		TEST_ASSERT_EQUAL(len, fwrite(input, 1, len, tmp));
	}

	rewind(tmp);
	TEST_ASSERT_NOT_EQUAL(-1, dup2(fileno(tmp), fileno(stdin)));

	result = list_contacts(book, "Test", index, NULL, e_list);

	TEST_ASSERT_NOT_EQUAL(-1, dup2(saved_stdin_fd, fileno(stdin)));
	close(saved_stdin_fd);
	fclose(tmp);

	return result;
}

/* ---------- setUp / tearDown ---------- */

void setUp(void) {}
void tearDown(void) {}

/* ---------- tests ---------- */

void test_null_book(void)
{
	int index = 0;
	Status result = list_contacts(NULL, "Test", &index, NULL, e_list);
	TEST_ASSERT_EQUAL_INT(e_fail, result);
}

void test_null_list(void)
{
	AddressBook book = { NULL, NULL, 1 };
	int index = 0;
	Status result = list_contacts(&book, "Test", &index, NULL, e_list);
	TEST_ASSERT_EQUAL_INT(e_fail, result);
}

void test_zero_count(void)
{
	ContactInfo dummy;
	AddressBook book;
	book.fp    = NULL;
	book.list  = &dummy;
	book.count = 0;
	int index = 0;
	Status result = list_contacts(&book, "Test", &index, NULL, e_list);
	TEST_ASSERT_EQUAL_INT(e_fail, result);
}

void test_quit_returns_success(void)
{
	AddressBook book = make_contacts(1);
	int index = 0;
	Status result = run_list_contacts(&book, &index, "Q\n");
	TEST_ASSERT_EQUAL_INT(e_success, result);
	free(book.list);
}

void test_index_unchanged_after_quit(void)
{
	AddressBook book = make_contacts(1);
	int index = 0;
	run_list_contacts(&book, &index, "Q\n");
	TEST_ASSERT_EQUAL_INT(0, index);
	free(book.list);
}

void test_next_advances_index(void)
{
	AddressBook book = make_contacts(6);
	int index = 0;
	run_list_contacts(&book, &index, "N\nQ\n");
	TEST_ASSERT_EQUAL_INT(WINDOW_SIZE, index);
	free(book.list);
}

void test_prev_ignored_on_first_page(void)
{
	AddressBook book = make_contacts(6);
	int index = 0;
	run_list_contacts(&book, &index, "P\nQ\n");
	TEST_ASSERT_EQUAL_INT(0, index);
	free(book.list);
}

void test_next_then_prev_returns_to_start(void)
{
	AddressBook book = make_contacts(6);
	int index = 0;
	run_list_contacts(&book, &index, "N\nP\nQ\n");
	TEST_ASSERT_EQUAL_INT(0, index);
	free(book.list);
}

/* ---------- main ---------- */

int main(void)
{
	UNITY_BEGIN();

	RUN_TEST(test_null_book);
	RUN_TEST(test_null_list);
	RUN_TEST(test_zero_count);
	RUN_TEST(test_quit_returns_success);
	RUN_TEST(test_index_unchanged_after_quit);
	RUN_TEST(test_next_advances_index);
	RUN_TEST(test_prev_ignored_on_first_page);
	RUN_TEST(test_next_then_prev_returns_to_start);

	return UNITY_END();
}
