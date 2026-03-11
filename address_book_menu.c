//#include <cstdio>
//#include <cstring>
#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"
#include "address_book_fops.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

//Ish and Chris
int get_option(int type, const char *msg)
{
  char input[64];
  int index = 0;

  	if (msg != NULL && *msg != '\0'){ //Checks to see if msg is NULL
        printf("%s", msg);
	}

    if (fgets(input, sizeof(input), stdin) == NULL){ //Checks to see if input was NULL
        return e_no_opt;
    }

  // Read character input
  // Used for "Y", "N", "Q" for quit, "N" for next, etc...
  if (type == CHAR)
  {
    while (input[index] != '\0')
    {
      if(!isspace((unsigned char)input[index]))
      {
        return toupper((unsigned char)input[index]);
      }
      index++;
    }
  }

  // Read number input
  // Used for Menu options (add contact, edit contact, etc...)
  if (type == NUM)
  {
    while (input[index] != '\0')
    {
      if (isdigit((unsigned char)input[index]))
      {
        return input[index] - '0';
      }
      index++;
    }
    return e_no_opt;
  }

  // Read no input
  // Used for "hit enter to continue"
  if (type == NONE)
  {
    return e_no_opt;
  }

  return e_no_opt;
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

//Chris
Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
  if (address_book == NULL || address_book->list == NULL || address_book->count == 0)
  {
    return e_fail;
  }

  char option;

  do
  {
    menu_header(title);

    if (msg != NULL && *msg != '\0')
    {
      printf("%s\n", msg);
    }

    int start = *index;

    int end = start + WINDOW_SIZE;
    if (end > address_book->count)
    {
      end = address_book->count;
    }

    for (int i = start; i < end; i++) {
      ContactInfo *c = &address_book->list[i];
      printf("\n[%d] %s\n", c->si_no, c->name[0]);

      for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
        if (strlen(c->phone_numbers[j]) > 0)
        {
          printf(" Phone: %s\n", c->phone_numbers[j]);
        }
      }

      for (int j = 0; j < EMAIL_ID_COUNT; j++) {
        if (strlen(c->email_addresses[j]) > 0)
        {
          printf(" Email: %s\n", c->email_addresses[j]);
        }
      }
    }

    int total_pages = (address_book->count + WINDOW_SIZE - 1) / WINDOW_SIZE;
    int current_page = (*index / WINDOW_SIZE) +1;
    printf("\n--- Page %d of %d---\n", current_page, total_pages);

    if (*index + WINDOW_SIZE < address_book->count)
    {
      printf("N. Next\n");
    }

    if (*index > 0)
    {
      printf("P. Previous\n");
    }

    printf("Q. Quit\n");

    option = get_option(CHAR, "Select: ");

    if (option == 'N' && *index + WINDOW_SIZE < address_book->count)
    {
      *index += WINDOW_SIZE;
    }else if (option == 'P' && *index > 0)
    {
      *index -= WINDOW_SIZE;
    }

  } while (option != 'Q');

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str != NULL)
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				ret = add_contacts(address_book);
    			if (ret == e_success){
       		 		printf("Contact added successfully.\n");
    			}
    			else{
    			    printf("Failed to add contact.\n");
   				 }
    			break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				/* Add your implementation to call list_contacts function here */
				{
					int idx = 0;
					list_contacts(address_book, "List Contacts", &idx, NULL, e_list);
				}
				break;
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

//Ismael
Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */

	int check = 0; //Used as a way to see if the function is done looping
	ContactInfo *temp = realloc(address_book->list,(address_book->count + 1) * sizeof(ContactInfo)); //Reallocating Memory for each new contact added.

	if (temp == NULL){ //Checks if the reallocating was successful
		return e_fail;
	}

	address_book->list = temp; //Assign the new memory back to the address book

	ContactInfo *new_contact = &address_book->list[address_book->count]; //New contact pointer using the address books count to see whats the next
																		//available slot
	memset(new_contact,0,sizeof(ContactInfo)); //Clears the memory so unused phone numbers and emails start empty

	printf("Enter Name: "); //Gets the name from  the user
	fgets(new_contact->name[0],NAME_LEN, stdin);
	new_contact->name[0][strcspn(new_contact->name[0],"\n")] = '\0'; //Fixs the input of the \n because of the fgets

	//Check if the user actually entered a name
	if(strlen(new_contact->name[0]) == 0){
		printf("Name cannot be empty.\n");
		return e_fail;
	}

	for (int i = 0; i <PHONE_NUMBER_COUNT && check == 0; i++){
		printf("Enter Phone %d (press enter to skip): ",i + 1);//Adds phone numbers to one contact
		fgets(new_contact->phone_numbers[i],NUMBER_LEN,stdin);
		new_contact->phone_numbers[i][strcspn(new_contact->phone_numbers[i], "\n")] = '\0';

		if (strlen(new_contact->phone_numbers[i]) == 0){ //Leaves the loop
			check = 1;
		}

	}

	check = 0; //Check back to zero

	for (int i = 0; i < EMAIL_ID_COUNT && check == 0; i++){ //Same as the Phone number
		printf("Enter Email %d (press enter to stop): ", i + 1);
        fgets(new_contact->email_addresses[i], EMAIL_ID_LEN, stdin);
        new_contact->email_addresses[i][strcspn(new_contact->email_addresses[i], "\n")] = '\0';

        if (strlen(new_contact->email_addresses[i]) == 0){
            check = 1;
        }
	}

	new_contact->si_no = address_book->count + 1; //Gives the contact a serial number
    address_book->count++; //Increase count
    return e_success;
}

//Ismael
Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
	int found = 0;
	int match = 0;

	if (str == NULL || address_book == NULL || address_book->list == NULL){ //Check for NULL Values
		return e_fail;
	}

	for (int i = 0; i < address_book->count; i++){
		//Match needs to be set to zero since the user might want to do multiple searches
		match = 0;
		if (field == 0){ //Search by name
			if (strcmp(address_book->list[i].name[0], str) == 0){
				match = 1;
			}
		}

		else if (field == 1){ //Search by phone number
			for (int j = 0; j < PHONE_NUMBER_COUNT && match == 0; j++){
				if (strcmp(address_book->list[i].phone_numbers[j], str) == 0){
					match = 1;
				}
			}
		}

		else if (field == 2){ //Search via email
			for (int j = 0; j < EMAIL_ID_COUNT && match == 0; j++){
				if (strcmp(address_book->list[i].email_addresses[j], str) == 0){
					match = 1;
				}
			}
		}

		if (match == 1){
			printf("Contact Found:\n");
			printf("S.No : %d\n", address_book->list[i].si_no); // Prints it serial number
			printf("Name : %s\n", address_book->list[i].name[0]); //Prints the name

			printf("Phone Numbers:\n");
			for (int j = 0; j < PHONE_NUMBER_COUNT; j++){ // Prints the Multiple phone numbers if any
				if (strlen(address_book->list[i].phone_numbers[j]) > 0){
					printf("%s\n", address_book->list[i].phone_numbers[j]);
				}
			}

			printf("Email Addresses:\n");
			for (int j = 0; j < EMAIL_ID_COUNT; j++){ // Prints the Multiple emails if any
				if (strlen(address_book->list[i].email_addresses[j]) > 0){
					printf("%s\n", address_book->list[i].email_addresses[j]);
				}
			}
			//Needed at the end so it doesn't stay zero and return no match
			found = 1;
		}
	}
	//Nothing found
	if (found == 0){
		return e_no_match;
	}
	return e_success;
}

//This function is the interactive part the user uses for the search the one
//above is the logic.

//Ricardo
Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	// error handling, making sure our address book actually exists
	if (address_book == NULL || address_book->list == NULL || address_book->count == 0) {
		printf("No contacts found.\n");
		return e_fail;
	}
	
	//print menu heading for searching
	menu_header("Search Contact to Delete by:\n\n");
	//Menu
	printf("Search by: \n");
	printf("1: Name \n");
	printf("2: Phone Number \n");
	printf("3: Email \n");
	printf("4: Session ID \n");
	
	//Ask the user to give an option and receive 
	int fieldOption = get_option(NUM, "Please select an option: ");
	

	// field will be fed into search function
	//prompt will be shown to user so that they can input their search query
	//SEARCH_LEN will be the length cap of query
	int field;
	const char *prompt;
	int SEARCH_LEN;

	// switch case to prepare SEARCH_LEN, prompt, and field for the user.
	switch(fieldOption) {
		case 1:
			field = 0;
			prompt = "Please enter a name to search: ";
			SEARCH_LEN = NAME_LEN;
			break;
		case 2:
			field = 1;
			prompt = "Please enter a phone number to search: ";
			SEARCH_LEN = 10;
			break;
		case 3:
			field = 2;
			prompt = "Please enter an email address to search: ";
			SEARCH_LEN = EMAIL_ID_LEN;
			break;
		case 4:
			field = 3;
			prompt = "Please enter a session ID to search: ";
			SEARCH_LEN = NAME_LEN;
			break;
		default:
			prompt = "Invalid option.\n";
			return e_fail;
	}

	// removing newline and replacing with null terminator
	char query[SEARCH_LEN];
	printf("%s", prompt);
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';

	//error handling is empty query
	if (strlen(query) == 0) {
		printf("Search query cannot be empty.\n");
		return e_fail;
	}

	Status result = search(query, address_book, address_book->count, field, NULL, e_search);

	if (result == e_no_match) {
		printf("No contacts found matching \"%s\".\n", query);
	}

	get_option(NONE, "\nPress Enter to continue...");

	return result;
}

//Viknesh Ashokkumar
Status edit_contact(AddressBook *address_book)
{
    if (address_book == NULL || address_book->list == NULL || address_book->count == 0) {
        printf("No contacts to edit.\n");
        get_option(NONE, "\nPress Enter to continue...");
        return e_fail;
    }

    menu_header("Edit Contact:\n");
    printf("0. Back\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone\n");
    printf("3. Search by Email\n");
    printf("\n");

    int option = get_option(NUM, "Please select an option: ");

    if (option == 0) return e_back;

    int field;
    const char *prompt;
    int search_len;

    switch (option) {
        case 1: field = 0; prompt = "Enter name to search: ";  search_len = NAME_LEN;     break;
        case 2: field = 1; prompt = "Enter phone to search: "; search_len = NUMBER_LEN;   break;
        case 3: field = 2; prompt = "Enter email to search: "; search_len = EMAIL_ID_LEN; break;
        default: printf("Invalid option.\n"); return e_fail;
    }

    char query[NAME_LEN];
    printf("%s", prompt);
    fgets(query, search_len, stdin);
    query[strcspn(query, "\n")] = '\0';

    if (strlen(query) == 0) {
        printf("Query cannot be empty.\n");
        return e_fail;
    }

    // Find the matching contact
    int found_index = -1;
    for (int i = 0; i < address_book->count; i++) {
        ContactInfo *c = &address_book->list[i];
        if (field == 0 && strcmp(c->name[0], query) == 0) { found_index = i; break; }
        if (field == 1) {
            for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
                if (strcmp(c->phone_numbers[j], query) == 0) { found_index = i; break; }
            }
            if (found_index != -1) break;
        }
        if (field == 2) {
            for (int j = 0; j < EMAIL_ID_COUNT; j++) {
                if (strcmp(c->email_addresses[j], query) == 0) { found_index = i; break; }
            }
            if (found_index != -1) break;
        }
    }

    if (found_index == -1) {
        printf("No contact found matching \"%s\".\n", query);
        get_option(NONE, "\nPress Enter to continue...");
        return e_no_match;
    }

    ContactInfo *c = &address_book->list[found_index];

    // Show edit sub-menu for that contact
    int edit_option;
    do {
        menu_header("Edit Contact:\n");
        printf("0. Back\n");
        printf("1. Name     : %s\n", c->name[0]);

        for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
            printf("%d. Phone %d  : %s\n", j + 2, j + 1, c->phone_numbers[j]);
        }
        for (int j = 0; j < EMAIL_ID_COUNT; j++) {
            printf("%d. Email %d  : %s\n", j + 2 + PHONE_NUMBER_COUNT, j + 1, c->email_addresses[j]);
        }
        printf("\n");

        edit_option = get_option(NUM, "Select field to edit: ");

        if (edit_option == 0) break;

        if (edit_option == 1) {
            printf("Enter new name: ");
            fgets(c->name[0], NAME_LEN, stdin);
            c->name[0][strcspn(c->name[0], "\n")] = '\0';
            printf("Name updated.\n");
        }
        else if (edit_option >= 2 && edit_option < 2 + PHONE_NUMBER_COUNT) {
            int idx = edit_option - 2;
            printf("Enter new phone (leave blank to clear): ");
            fgets(c->phone_numbers[idx], NUMBER_LEN, stdin);
            c->phone_numbers[idx][strcspn(c->phone_numbers[idx], "\n")] = '\0';
            printf("Phone updated.\n");
        }
        else if (edit_option >= 2 + PHONE_NUMBER_COUNT && edit_option < 2 + PHONE_NUMBER_COUNT + EMAIL_ID_COUNT) {
            int idx = edit_option - 2 - PHONE_NUMBER_COUNT;
            printf("Enter new email (leave blank to clear): ");
            fgets(c->email_addresses[idx], EMAIL_ID_LEN, stdin);
            c->email_addresses[idx][strcspn(c->email_addresses[idx], "\n")] = '\0';
            printf("Email updated.\n");
        }

        get_option(NONE, "\nPress Enter to continue...");

    } while (edit_option != 0);

    return e_success;
}

// Viknesh Ashokkumar
Status delete_contact(AddressBook *address_book)
{
    if (address_book == NULL || address_book->list == NULL || address_book->count == 0) {
        printf("No contacts to delete.\n");
        get_option(NONE, "\nPress Enter to continue...");
        return e_fail;
    }

    menu_header("Delete Contact:\n");
    printf("0. Back\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone\n");
    printf("3. Search by Email\n");
    printf("\n");

    int option = get_option(NUM, "Please select an option: ");

    if (option == 0) return e_back;

    int field;
    const char *prompt;
    int search_len;

    switch (option) {
        case 1: field = 0; prompt = "Enter name to delete: ";  search_len = NAME_LEN;     break;
        case 2: field = 1; prompt = "Enter phone to delete: "; search_len = NUMBER_LEN;   break;
        case 3: field = 2; prompt = "Enter email to delete: "; search_len = EMAIL_ID_LEN; break;
        default: printf("Invalid option.\n"); return e_fail;
    }

    char query[NAME_LEN];
    printf("%s", prompt);
    fgets(query, search_len, stdin);
    query[strcspn(query, "\n")] = '\0';

    if (strlen(query) == 0) {
        printf("Query cannot be empty.\n");
        return e_fail;
    }

    // Find matching contact
    int found_index = -1;
    for (int i = 0; i < address_book->count; i++) {
        ContactInfo *c = &address_book->list[i];
        if (field == 0 && strcmp(c->name[0], query) == 0) { found_index = i; break; }
        if (field == 1) {
            for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
                if (strcmp(c->phone_numbers[j], query) == 0) { found_index = i; break; }
            }
            if (found_index != -1) break;
        }
        if (field == 2) {
            for (int j = 0; j < EMAIL_ID_COUNT; j++) {
                if (strcmp(c->email_addresses[j], query) == 0) { found_index = i; break; }
            }
            if (found_index != -1) break;
        }
    }

    if (found_index == -1) {
        printf("No contact found matching \"%s\".\n", query);
        get_option(NONE, "\nPress Enter to continue...");
        return e_no_match;
    }

    ContactInfo *c = &address_book->list[found_index];

    // Show the contact about to be deleted
    menu_header("Delete Contact:\n");
    printf("Contact to delete:\n");
    printf("Name  : %s\n", c->name[0]);
    for (int j = 0; j < PHONE_NUMBER_COUNT; j++) {
        if (strlen(c->phone_numbers[j]) > 0)
            printf("Phone : %s\n", c->phone_numbers[j]);
    }
    for (int j = 0; j < EMAIL_ID_COUNT; j++) {
        if (strlen(c->email_addresses[j]) > 0)
            printf("Email : %s\n", c->email_addresses[j]);
    }

    char confirm = get_option(CHAR, "\nEnter 'Y' to delete (any other key to cancel): ");

    if (confirm != 'Y') {
        printf("Delete cancelled.\n");
        get_option(NONE, "\nPress Enter to continue...");
        return e_success;
    }

    // Shift all contacts after this one left by one
    for (int i = found_index; i < address_book->count - 1; i++) {
        address_book->list[i] = address_book->list[i + 1];
        address_book->list[i].si_no = i + 1; // re-number
    }

    address_book->count--;

    // Shrink the memory
    if (address_book->count > 0) {
        ContactInfo *temp = realloc(address_book->list, address_book->count * sizeof(ContactInfo));
        if (temp != NULL) address_book->list = temp;
    } else {
        free(address_book->list);
        address_book->list = NULL;
    }

    printf("Contact deleted successfully.\n");
    get_option(NONE, "\nPress Enter to continue...");

    return e_success;
}
