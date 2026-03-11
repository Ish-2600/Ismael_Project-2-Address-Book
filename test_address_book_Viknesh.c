#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "address_book.h"
#include "address_book_menu.h"

void setUp(void) {}
void tearDown(void) {}

static AddressBook make_book(int count)
{
    AddressBook ab;
    ab.fp = NULL;
    ab.count = count;
    ab.list = (ContactInfo *)malloc(count * sizeof(ContactInfo));

    for (int i = 0; i < count; i++) {
        memset(&ab.list[i], 0, sizeof(ContactInfo));
        snprintf(ab.list[i].name[0], NAME_LEN, "Contact%d", i + 1);
        snprintf(ab.list[i].phone_numbers[0], NUMBER_LEN, "555000%04d", i + 1);
        snprintf(ab.list[i].email_addresses[0], EMAIL_ID_LEN, "contact%d@test.com", i + 1);
        ab.list[i].si_no = i + 1;
    }
    return ab;
}

static void free_book(AddressBook *ab)
{
    free(ab->list);
    ab->list = NULL;
    ab->count = 0;
}

static Status do_delete(AddressBook *ab, int idx)
{
    for (int i = idx; i < ab->count - 1; i++) {
        ab->list[i] = ab->list[i + 1];
        ab->list[i].si_no = i + 1;
    }
    ab->count--;

    if (ab->count > 0) {
        ContactInfo *tmp = realloc(ab->list, ab->count * sizeof(ContactInfo));
        if (tmp != NULL) ab->list = tmp;
    } else {
        free(ab->list);
        ab->list = NULL;
    }
    return e_success;
}

static Status do_edit_name(AddressBook *ab, int idx, const char *newname)
{
    if (idx < 0 || idx >= ab->count) return e_fail;
    strncpy(ab->list[idx].name[0], newname, NAME_LEN - 1);
    ab->list[idx].name[0][NAME_LEN - 1] = '\0';
    return e_success;
}

static Status do_edit_phone(AddressBook *ab, int idx, int slot, const char *newphone)
{
    if (idx < 0 || idx >= ab->count) return e_fail;
    if (slot < 0 || slot >= PHONE_NUMBER_COUNT) return e_fail;
    strncpy(ab->list[idx].phone_numbers[slot], newphone, NUMBER_LEN - 1);
    ab->list[idx].phone_numbers[slot][NUMBER_LEN - 1] = '\0';
    return e_success;
}

static Status do_edit_email(AddressBook *ab, int idx, int slot, const char *newemail)
{
    if (idx < 0 || idx >= ab->count) return e_fail;
    if (slot < 0 || slot >= EMAIL_ID_COUNT) return e_fail;
    strncpy(ab->list[idx].email_addresses[slot], newemail, EMAIL_ID_LEN - 1);
    ab->list[idx].email_addresses[slot][EMAIL_ID_LEN - 1] = '\0';
    return e_success;
}

void test_delete_one_contact(void)
{
    AddressBook ab = make_book(1);
    Status ret = do_delete(&ab, 0);
    TEST_ASSERT_EQUAL(e_success, ret);
    TEST_ASSERT_EQUAL_INT(0, ab.count);
    TEST_ASSERT_NULL(ab.list);
}

void test_delete_middle(void)
{
    AddressBook ab = make_book(3);
    do_delete(&ab, 1);
    TEST_ASSERT_EQUAL_INT(2, ab.count);
    TEST_ASSERT_EQUAL_STRING("Contact1", ab.list[0].name[0]);
    TEST_ASSERT_EQUAL_STRING("Contact3", ab.list[1].name[0]);
    free_book(&ab);
}

void test_delete_first(void)
{
    AddressBook ab = make_book(3);
    do_delete(&ab, 0);
    TEST_ASSERT_EQUAL_INT(2, ab.count);
    TEST_ASSERT_EQUAL_STRING("Contact2", ab.list[0].name[0]);
    free_book(&ab);
}

void test_delete_last(void)
{
    AddressBook ab = make_book(3);
    do_delete(&ab, 2);
    TEST_ASSERT_EQUAL_INT(2, ab.count);
    TEST_ASSERT_EQUAL_STRING("Contact2", ab.list[1].name[0]);
    free_book(&ab);
}

void test_delete_empty_book(void)
{
    AddressBook ab;
    ab.list = NULL;
    ab.count = 0;
    ab.fp = NULL;
    Status ret = (ab.list == NULL || ab.count == 0) ? e_fail : e_success;
    TEST_ASSERT_EQUAL(e_fail, ret);
}

void test_edit_name(void)
{
    AddressBook ab = make_book(2);
    Status ret = do_edit_name(&ab, 0, "NewName");
    TEST_ASSERT_EQUAL(e_success, ret);
    TEST_ASSERT_EQUAL_STRING("NewName", ab.list[0].name[0]);
    free_book(&ab);
}

void test_edit_phone(void)
{
    AddressBook ab = make_book(2);
    Status ret = do_edit_phone(&ab, 0, 0, "9998887777");
    TEST_ASSERT_EQUAL(e_success, ret);
    TEST_ASSERT_EQUAL_STRING("9998887777", ab.list[0].phone_numbers[0]);
    free_book(&ab);
}

void test_edit_email(void)
{
    AddressBook ab = make_book(2);
    Status ret = do_edit_email(&ab, 0, 0, "new@email.com");
    TEST_ASSERT_EQUAL(e_success, ret);
    TEST_ASSERT_EQUAL_STRING("new@email.com", ab.list[0].email_addresses[0]);
    free_book(&ab);
}

void test_edit_only_changes_target(void)
{
    AddressBook ab = make_book(3);
    do_edit_name(&ab, 1, "Changed");
    TEST_ASSERT_EQUAL_STRING("Contact1", ab.list[0].name[0]);
    TEST_ASSERT_EQUAL_STRING("Changed", ab.list[1].name[0]);
    TEST_ASSERT_EQUAL_STRING("Contact3", ab.list[2].name[0]);
    free_book(&ab);
}

void test_edit_bad_index(void)
{
    AddressBook ab = make_book(2);
    Status ret = do_edit_name(&ab, 99, "Ghost");
    TEST_ASSERT_EQUAL(e_fail, ret);
    free_book(&ab);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_delete_one_contact);
    RUN_TEST(test_delete_middle);
    RUN_TEST(test_delete_first);
    RUN_TEST(test_delete_last);
    RUN_TEST(test_delete_empty_book);
    RUN_TEST(test_edit_name);
    RUN_TEST(test_edit_phone);
    RUN_TEST(test_edit_email);
    RUN_TEST(test_edit_only_changes_target);
    RUN_TEST(test_edit_bad_index);
    return UNITY_END();
}
