#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "library.dat"

typedef struct {
    int id;
    char title[100];
    char author[100];
    int quantity;
} Book;

void addBook();
void displayBooks();
void searchBook();
void deleteBook();
void updateBook();
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    while (1) {
        printf("\n===== Library Management System =====\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Update Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: updateBook(); break;
            case 6: exit(0);
            default: printf("\nInvalid choice! Try again.\n");
        }
    }
}

void addBook() {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Book b;
    printf("\nEnter Book ID: ");
    scanf("%d", &b.id);
    getchar(); // to consume newline
    printf("Enter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0; // remove newline
    printf("Enter Author Name: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;
    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);

    fwrite(&b, sizeof(Book), 1, fp);
    fclose(fp);
    printf("\nBook added successfully!\n");
}

void displayBooks() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    Book b;
    printf("\n===== Book List =====\n");
    printf("%-5s %-30s %-20s %-10s\n", "ID", "Title", "Author", "Quantity");
    printf("-------------------------------------------------------------\n");

    while (fread(&b, sizeof(Book), 1, fp) == 1) {
        printf("%-5d %-30s %-20s %-10d\n", b.id, b.title, b.author, b.quantity);
    }

    fclose(fp);
}

void searchBook() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    int searchID;
    char searchTitle[100];
    int choice, found = 0;
    Book b;

    printf("\nSearch by:\n1. ID\n2. Title\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Book ID: ");
        scanf("%d", &searchID);
        while (fread(&b, sizeof(Book), 1, fp) == 1) {
            if (b.id == searchID) {
                printf("\nBook Found!\nID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\n",
                       b.id, b.title, b.author, b.quantity);
                found = 1;
                break;
            }
        }
    } else if (choice == 2) {
        getchar();
        printf("Enter Book Title: ");
        fgets(searchTitle, sizeof(searchTitle), stdin);
        searchTitle[strcspn(searchTitle, "\n")] = 0;
        while (fread(&b, sizeof(Book), 1, fp) == 1) {
            if (strcasecmp(b.title, searchTitle) == 0) {
                printf("\nBook Found!\nID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\n",
                       b.id, b.title, b.author, b.quantity);
                found = 1;
                break;
            }
        }
    } else {
        printf("Invalid choice!\n");
    }

    if (!found) {
        printf("\nBook not found!\n");
    }

    fclose(fp);
}

void deleteBook() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    int deleteID, found = 0;
    Book b;

    printf("\nEnter Book ID to delete: ");
    scanf("%d", &deleteID);

    while (fread(&b, sizeof(Book), 1, fp) == 1) {
        if (b.id != deleteID) {
            fwrite(&b, sizeof(Book), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("\nBook deleted successfully!\n");
    else
        printf("\nBook not found!\n");
}

void updateBook() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    int updateID, found = 0;
    Book b;

    printf("\nEnter Book ID to update: ");
    scanf("%d", &updateID);

    while (fread(&b, sizeof(Book), 1, fp) == 1) {
        if (b.id == updateID) {
            printf("\nCurrent Details: ID: %d, Title: %s, Author: %s, Quantity: %d\n",
                   b.id, b.title, b.author, b.quantity);

            getchar();
            printf("Enter New Title: ");
            fgets(b.title, sizeof(b.title), stdin);
            b.title[strcspn(b.title, "\n")] = 0;

            printf("Enter New Author: ");
            fgets(b.author, sizeof(b.author), stdin);
            b.author[strcspn(b.author, "\n")] = 0;

            printf("Enter New Quantity: ");
            scanf("%d", &b.quantity);

            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);

            found = 1;
            printf("\nBook updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("\nBook not found!\n");
    }

    fclose(fp);
}
