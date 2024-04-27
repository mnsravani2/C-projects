#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define WEEK_IN_SECONDS (7 * 24 * 60 * 60) // 1 week in seconds

struct Book 
{
	char name[50];
	char author[50];
	int copies;
};

void displayMenu() 
{
	printf("\n");
	printf("Select your role:\n");
	printf("1. User\n");
	printf("2. Admin\n");
	printf("3. Exit\n");
	printf("\n");
}

//function to write data to a file
void writeBooksToFile(struct Book books[], int count) 
{
	FILE *fp;
	fp = fopen("books.txt", "w");
	if (fp == NULL) 
	{
		printf("Error opening file.\n");
		return;
	}
	fwrite(books, sizeof(struct Book), count, fp);
	fclose(fp);
}

//function to add books in the library
void addBook(struct Book books[], int *count) 
{
	char name[50];
	int i, copies;
	printf("\nEnter the book name: ");
	scanf("%s", name);
	// check if book with same name already exists
	  for (i = 0; i < *count; i++) {
		  if (strcmp(books[i].name, name) == 0) {
			  printf("\nA book with this name already exists. Cannot add a duplicate book.\n");
			  return;
		  }
	  }
	  // book with same name does not exist, add new book
	  strcpy(books[*count].name, name);
	  printf("Enter the author name: ");
	  scanf("%s", books[*count].author);
	  printf("Enter the number of copies: ");
	  while (scanf("%d%*c", &copies) != 1) {
		  printf("\nInvalid input for number of copies. Please enter an integer.\n");
		  printf("Enter the number of copies: ");
		  // consume any extra characters in the input buffer
		  while (getchar() != '\n');
	  }
	  books[*count].copies = copies;
	  (*count)++;
	  // write book data to file
	  writeBooksToFile(books, *count);
}

//function to edit the books
void editBook(struct Book books[], int count) {
	char name[50], author[50], new_name[50];
	int i, copies, choice;
	printf("\nEnter the name of the book to edit: ");
	scanf("%s", name);
	for (i = 0; i < count; i++) {
		if (strcmp(books[i].name, name) == 0) {
			printf("\nWhat do you want to edit?\n");
			printf("1. Name\n");
			printf("2. Author\n");
			printf("3. Number of copies\n");
			printf("Enter your choice: ");
			scanf("%d", &choice);
			switch(choice) {
				case 1:
					printf("\nEnter the new name of the book: ");
					scanf("%s", new_name);
					// check if new name already exists																	
					for (int j = 0; j < count; j++) {
						if (strcmp(books[j].name, new_name) == 0) {
							printf("\nA book with the same name already exists.\n");
							return;
						}
					}
					strcpy(books[i].name, new_name);
					break;
				case 2:
					printf("\nEnter the new author of the book: ");
					scanf("%s", books[i].author);
					break;
				case 3:
					printf("\nEnter the new number of copies: ");
					scanf("%d", &copies);
					books[i].copies = copies;
					break;
				default:
					printf("\nInvalid choice.\n");
					return;
			}
			printf("\nBook details updated successfully.\n");
			return;
		}
	}
	printf("\nBook not found.\n");
}

//function used to remove the books
void removeBook(struct Book books[], int *count) 
{
	char name[50];
	int i, j;
	printf("\nEnter the name of the book to remove: ");
	scanf("%s", name);
	for (i = 0; i < *count; i++) 
	{
		if (strcmp(books[i].name, name) == 0) 
		{
			for (j = i; j < (*count) - 1; j++) 
			{
				strcpy(books[j].name, books[j + 1].name);
				strcpy(books[j].author, books[j + 1].author);
				books[j].copies = books[j + 1].copies;
			}
			(*count)--;
			printf("\nBook removed successfully.\n");
			return;
		}
	}
	printf("\nBook not found.\n");
}

//function the erase the data from the file
void eraseFile() 
{
	int status = remove("books.txt");
	if (status == 0) 
	{
		printf("File deleted successfully.\n");
	}
       	else 
	{
		printf("Error deleting file.\n");
	}
}

//function to search books in the library
void searchBook(struct Book books[], int count) 
{
	char name[50];
	int i;
	printf("\nEnter the name of the book to search: ");
	scanf("%s", name);
	for (i = 0; i < count; i++) 
	{
		if (strcmp(books[i].name, name) == 0) 
		{
			printf("\nBook details:\n");
			printf("Name: %s\n", books[i].name);
			printf("Author: %s\n", books[i].author);
			printf("Copies: %d\n", books[i].copies);
			return;
		}
	}
	printf("\nBook not found.\n");
}

//function to checkout the books
void checkoutBook(struct Book books[], int count) 
{
	char name[50];
	int i;
	printf("\nEnter the name of the book to checkout: ");
	scanf("%s", name);
	for (i = 0; i < count; i++) 
	{
		if (strcmp(books[i].name, name) == 0) 
		{
			if (books[i].copies > 0) 
			{
				books[i].copies--;
				printf("\nBook checked out successfully.\n");
				return;
			}
			else 
			{
				printf("\nNo copies of the book are available.\n");
				return;
			}
		}
	}
	printf("\nBook not found.\n");
}

//function to checkin the books
void checkinBook(struct Book books[], int count) 
{
	char name[50];
	int i;
	printf("\nEnter the name of the book to checkin: ");
	scanf("%s", name);
	for (i = 0; i < count; i++) 
	{
		if (strcmp(books[i].name, name) == 0) 
		{
			books[i].copies++;
			printf("\nBook checked in successfully.\n");
			return;
		}
	}
	printf("\nBook not found.\n");
}

//function to display all the books
void displayBooks(struct Book books[], int count) 
{
	int i;
	printf("\nList of books:\n");
	printf("\n-------------------------------------------------------------------------\n");
	printf("%-30s%-30s%-10s\n", "Name", "Author", "Copies");
	printf("\n-------------------------------------------------------------------------\n");
	for (i = 0; i < count; i++) 
	{
		printf("%-30s%-30s%-10d\n", books[i].name, books[i].author, books[i].copies);
	}
}

//function for usermenu
void userMenu(struct Book books[], int count) 
{
	int choice;
	do 
	{
		printf("\n");
		printf("Select an option:\n");
		printf("1. Search book\n");
		printf("2. Check out book\n");
		printf("3. Check in book\n");
		printf("4. Display all books\n");
		printf("5. Exit\n");
		printf("\n");
		scanf("%d", &choice);
		switch (choice) 
		{
			case 1:
				searchBook(books, count);
				break;
			case 2:
				checkoutBook(books, count);
				break;
			case 3:
				checkinBook(books, count);
				break;
			case 4:
				displayBooks(books, count);
				break;
			case 5:
				printf("\nExiting..\n");
				break;
			default:
				printf("\nInvalid choice. Please try again.\n");
		}
	} while (choice != 5);
}

//function for adminmenu
void adminMenu(struct Book books[], int *count) 
{
	int choice;
	do 
	{
		printf("\n");
		printf("Select an option:\n");
		printf("1. Add book\n");
		printf("2. Edit book\n");
		printf("3. Remove book\n");
		printf("4. Display all books\n");
		printf("5. Erase data\n");
		printf("6. Exit\n");
		printf("\n");
		scanf("%d", &choice);
		switch (choice) 
		{
			case 1:
				addBook(books, count);
				break;
			case 2:
				editBook(books, *count);
				break;
			case 3:
				removeBook(books, count);													
				break;
			case 4:
				displayBooks(books, *count);
				break;
			case 5:
				eraseFile();
				break;
			case 6:
				printf("\nExiting..\n");
				break;
			default:
				printf("\nInvalid choice. Please try again.\n");
		}
	} while (choice != 6);
}

//Main function
int main() 
{
	struct Book books[100];
	int count = 0;
	FILE *fp;
	fp = fopen("books.txt", "r");
	if (fp != NULL) 
	{
		while (fread(&books[count], sizeof(struct Book), 1, fp) == 1) 
		{
			count++;
		}
		fclose(fp);
	}
	int choice, i;
	do 
	{
		displayMenu();
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice) 
		{
			case 1:
				userMenu(books, count);
				break;
			case 2:
				adminMenu(books, &count);
				break;
			case 3:
				printf("\nExiting program.\n");
				break;
			default:
				printf("\nInvalid choice.\n");
				break;
		}
	} while (choice != 3);

	// write book data to file before exiting program
	writeBooksToFile(books, count);
	
	time_t last_backup_time = 0; // initial backup time is 0 
	time_t current_time;
	
	int backup_count = 0;

	// loop indefinitely
	while (1) {
		// get current time
	       current_time = time(NULL);
	       // check if a week has passed since the last backup
	        if (current_time - last_backup_time >= WEEK_IN_SECONDS) {
			// perform backup										
			   printf("Performing backup...\n");
			// code for backup goes here
			// update last backup time
			last_backup_time = current_time;
			// increment backup count
			   backup_count++;
			   printf("Backup completed. Total backups: %d\n", backup_count);
		}
		// wait for some time before checking again
		    sleep(60); // wait for 60 seconds
	}
	return 0;
}

