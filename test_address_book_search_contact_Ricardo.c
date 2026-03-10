#include <stdio.h>
#include "unity.h"
#include "address_book.h"
#include "address_book_menu.h"
#include <string.h>

void setUp(void) {

}

void tearDown(void) {

}

// Helper function, create mock address book for us
AddressBook makeTestAddressBook(void) {
    AddressBook ab;
    ab.count = 2;
    ab.list = malloc(2 * sizeof(ContactInfo));

    memset(&ab.list[0], 0, sizeof(ContactInfo));
    ab.list[0].si_no = 1;
    strcpy(ab.list[0].name, "John Doe");
    strcpy(ab.list[0].phone_numbers[0], "6261234567");
    strcpy(ab.list[0].email_addresses[0], "johndoe@gmail.com");

    memset(&ab.list[1], 0, sizeof(ContactInfo));
    strcpy(ab.list[1].name, "Jane Doe");
    strcpy(ab.list[1].phone_numbers[0], "62668901234");
    strcpy(ab.list[1].email_addresses[0], "janedoe@hotmail.com");

    return ab;
}


void test_search_name_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("John Doe", &ab, ab.count, 0, NULL, e_search);
    TEST_ASSERT_EQUAL(e_success, result); // We expect to find the name
    free(ab.list);
}

void test_search_name_not_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("None", &ab, ab.count, 0, NULL, e_search);
    TEST_ASSERT_EQUAL(e_no_match, result); // We expect to not find the name.
    free(ab.list);
}

void test_search_phone_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("6261234567", &ab, ab.count, 1, NULL, e_search);
    TEST_ASSERT_EQUAL(e_success, result); // We expect to find the phone number
    free(ab.list);
}

void test_search_phone_not_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("7777777777", &ab, ab.count, 1, NULL, e_search);
    TEST_ASSERT_EQUAL(e_no_match, result); // We expect to not find the phone number
    free(ab.list);
}

void test_search_email_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("janedoe@hotmail.com", &ab, ab.count, 2, NULL, e_search);
    TEST_ASSERT_EQUAL(e_success, result); // We expect to find the email
    free(ab.list);
}

void test_search_email_not_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("janedoe@gmail.com", &ab, ab.count, 2, NULL, e_search);
    TEST_ASSERT_EQUAL(e_no_match, result); // We expect to not find the email
    free(ab.list);
}

// still need to implement session
void test_search_session_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("1", &ab, ab.count, 3, NULL, e_search);
    TEST_ASSERT_EQUAL(e_success, result); // We expect to find the session id
    free(ab.list);
}

// still need to implement session
void test_search_session_not_found(void) {
    AddressBook ab = makeTestAddressBook();
    Status result = search("67", &ab, ab.count, 3, NULL, e_search);
    TEST_ASSERT_EQUAL(e_no_match, result); // We expect to find the session id
    free(ab.list);
}

void test_search_empty_book(void) {
    AddressBook ab;
    ab.count = 0;
    ab.list = NULL;

    Status result = search("John Doe", &ab, ab.count, 0, NULL, e_search);
    TEST_ASSERT_EQUAL(e_fail, result); // We expect to return a "e_fail"
    free(ab.list);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_search_name_found);
    RUN_TEST(test_search_name_not_found);
    RUN_TEST(test_search_phone_found);
    RUN_TEST(test_search_phone_not_found);
    RUN_TEST(test_search_email_found);
    RUN_TEST(test_search_email_not_found);
    //RUN_TEST(test_search_session_found);
    //RUN_TEST(test_search_session_not_found);
    RUN_TEST(test_search_empty_book);

    return UNITY_END();
}