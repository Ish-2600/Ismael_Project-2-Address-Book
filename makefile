all:
	gcc address_book_fops.c address_book_menu.c main.c -o addressbook -I.

debug1:
	gcc address_book_fops.c address_book_menu.c unity.c test_address_book_Ish.c -o test1 -I.

debug2:
	gcc address_book_fops.c address_book_menu.c unity.c test_address_book_get_option_Chris.c -o test2 -I.

debug3:
	gcc address_book_fops.c address_book_menu.c unity.c test_address_book_list_contacts_Chris.c -o test3 -I.
