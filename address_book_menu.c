#include <cstdio>
#include <cstring>
#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

int get_option(int type, const char *msg)
{
  char input[64];
  int index = 0;

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

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str != '\0')
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
				break;
				/* Add your implementation to call list_contacts function here */
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
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
