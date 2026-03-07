all:
	gcc -o address_book_menu_test address_book_menu_test.c address_book_menu.c address_book_fops.c unity.c && ./address_book_menu_test
