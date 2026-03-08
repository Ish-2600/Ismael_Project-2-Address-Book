#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "address_book.h"
#include "address_book_menu.h"

//Search function needs to be added here since its not in the header files, its a helper function
//, but it needs to be tested
Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode);

static AddressBook book;

// Redirect stdin to a test file
static void feed_input(const char *text){
    FILE *fp = fopen("test_input.txt", "w");
    fputs(text, fp);
    fclose(fp);

    freopen("test_input.txt", "r", stdin);
}

//Build one contact manually for search tests
static void make_one_contact(const char *name, const char *phone, const char *email){
    book.list = malloc(sizeof(ContactInfo));
    TEST_ASSERT_NOT_NULL(book.list);

    memset(&book.list[0], 0, sizeof(ContactInfo));

    strncpy(book.list[0].name[0], name, NAME_LEN - 1);
    strncpy(book.list[0].phone_numbers[0], phone, NUMBER_LEN - 1);
    strncpy(book.list[0].email_addresses[0], email, EMAIL_ID_LEN - 1);

    book.list[0].name[0][NAME_LEN - 1] = '\0';
    book.list[0].phone_numbers[0][NUMBER_LEN - 1] = '\0';
    book.list[0].email_addresses[0][EMAIL_ID_LEN - 1] = '\0';

    book.list[0].si_no = 1;
    book.count = 1;
    book.fp = NULL;
}

//Sets everything to zero for the test
//Unity uses for tests
void setUp(void){
    book.list = NULL;
    book.count = 0;
    book.fp = NULL;
}

void tearDown(void){
    free(book.list);
    book.list = NULL;
    book.count = 0;
    book.fp = NULL;
}

//The one contact added to the txt file to test the rest of the cases
//Also a test for the add fuction
void test_add_contacts_success(void){
    feed_input(
        "John Smith\n"
        "9091234567\n"
        "\n"
        "john@gmail.com\n"
        "\n"
    );

    TEST_ASSERT_EQUAL_INT(e_success, add_contacts(&book));
    TEST_ASSERT_EQUAL_INT(1, book.count);
    TEST_ASSERT_EQUAL_STRING("John Smith", book.list[0].name[0]);
    TEST_ASSERT_EQUAL_STRING("9091234567", book.list[0].phone_numbers[0]);
    TEST_ASSERT_EQUAL_STRING("john@gmail.com", book.list[0].email_addresses[0]);
    TEST_ASSERT_EQUAL_INT(1, book.list[0].si_no);
}

//Check to see if name is empty and fails
void test_add_contacts_empty_name_returns_fail(void){
    feed_input("\n");

    TEST_ASSERT_EQUAL_INT(e_fail, add_contacts(&book));
    TEST_ASSERT_EQUAL_INT(0, book.count);
}


//Next three functions are the same just texting each type of search
void test_search_by_name_found(void){
    make_one_contact("John Smith", "9091234567", "john@gmail.com");

    TEST_ASSERT_EQUAL_INT(
        e_success,
        search("John Smith", &book, 0, 0, NULL, e_search)
    );
}

void test_search_by_phone_found(void){
    make_one_contact("John Smith", "9091234567", "john@gmail.com");

    TEST_ASSERT_EQUAL_INT(
        e_success,
        search("9091234567", &book, 0, 1, NULL, e_search)
    );
}

void test_search_by_email_found(void){
    make_one_contact("John Smith", "9091234567", "john@gmail.com");

    TEST_ASSERT_EQUAL_INT(
        e_success,
        search("john@gmail.com", &book, 0, 2, NULL, e_search)
    );
}

//Search with no matchs
void test_search_returns_no_match(void){
    make_one_contact("John Smith", "9091234567", "john@gmail.com");

    TEST_ASSERT_EQUAL_INT(
        e_no_match,
        search("Maria", &book, 0, 0, NULL, e_search)
    );
}

int main(void){
    UNITY_BEGIN();
    
    RUN_TEST(test_add_contacts_success);
    RUN_TEST(test_add_contacts_empty_name_returns_fail);
    RUN_TEST(test_search_by_name_found);
    RUN_TEST(test_search_by_phone_found);
    RUN_TEST(test_search_by_email_found);
    RUN_TEST(test_search_returns_no_match);

    return UNITY_END();
}