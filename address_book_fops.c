#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{
    FILE *fp = fopen(DEFAULT_FILE, "r");

    address_book->fp = NULL;
    address_book->list = NULL;
    address_book->count = 0;

    if (fp == NULL)
        return e_success;

    int capacity = 10;
    address_book->list = malloc(sizeof(ContactInfo) * capacity);
    if (address_book->list == NULL)
    {
        fclose(fp);
        return e_fail;
    }

    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (line[0] == '\n' || line[0] == '\0')
            continue;

        line[strcspn(line, "\r\n")] = '\0';

        if (address_book->count >= capacity)
        {
            capacity *= 2;
            ContactInfo *tmp = realloc(address_book->list, sizeof(ContactInfo) * capacity);
            if (tmp == NULL)
            {
                free(address_book->list);
                address_book->list = NULL;
                address_book->count = 0;
                fclose(fp);
                return e_fail;
            }
            address_book->list = tmp;
        }

        ContactInfo *c = &address_book->list[address_book->count];

        char *tok = strtok(line, ",");
        if (!tok) continue;
        c->si_no = atoi(tok);

        tok = strtok(NULL, ",");
        if (!tok) continue;
        while (*tok == ' ') tok++;
        strncpy(c->name, tok, NAME_LEN - 1);
        c->name[NAME_LEN - 1] = '\0';

        tok = strtok(NULL, ",");
        if (!tok) continue;
        while (*tok == ' ') tok++;
        strncpy(c->phone_numbers, tok, NUMBER_LEN - 1);
        c->phone_numbers[NUMBER_LEN - 1] = '\0';

        tok = strtok(NULL, ",");
        if (!tok) continue;
        while (*tok == ' ') tok++;
        strncpy(c->email_addresses, tok, EMAIL_ID_LEN - 1);
        c->email_addresses[EMAIL_ID_LEN - 1] = '\0';

        address_book->count++;
    }

    fclose(fp);
    return e_success;
}





Status save_file(AddressBook *address_book)
{
    FILE *fp = fopen(DEFAULT_FILE, "w");
    if (fp == NULL)
        return e_fail;

    for (int i = 0; i < address_book->count; i++)
    {
        ContactInfo *c = &address_book->list[i];
        fprintf(fp, "%d,%s,%s,%s\n",
                c->si_no,
                c->name,
                c->phone_numbers,
                c->email_addresses);
    }

    fclose(fp);
    return e_success;
}
