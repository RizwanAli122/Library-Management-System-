#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book
{
    int id;
    char title[50];
    char author[50];
    int quantity;
};

struct Transaction
{
    int studentId;
    int bookId;
    int issueDay;
    int returnDay;
    int fine;
    int returned;
};

/* ---------------- BOOK FUNCTIONS ---------------- */

void addBook()
{
    struct Book b;

    FILE *fp = fopen("books.txt", "a");

    if(fp == NULL)
    {
        printf("File Error\n");
        return;
    }

    printf("Book ID: ");
    scanf("%d", &b.id);

    printf("Title: ");
    scanf(" %[^\n]", b.title);

    printf("Author: ");
    scanf(" %[^\n]", b.author);

    printf("Quantity: ");
    scanf("%d", &b.quantity);

    fprintf(fp, "%d|%s|%s|%d\n",
            b.id, b.title, b.author, b.quantity);

    fclose(fp);

    printf("Book Added Successfully\n");
}

void viewBooks()
{
    struct Book b;

    FILE *fp = fopen("books.txt", "r");

    if(fp == NULL)
    {
        printf("No Books Found\n");
        return;
    }

    printf("\nID\tTitle\tAuthor\tQty\n");

    while(fscanf(fp,"%d|%49[^|]|%49[^|]|%d\n",
                 &b.id,b.title,b.author,&b.quantity)==4)
    {
        printf("%d\t%s\t%s\t%d\n",
               b.id,b.title,b.author,b.quantity);
    }

    fclose(fp);
}

void searchBook()
{
    struct Book b;
    int id, found = 0;

    printf("Enter Book ID: ");
    scanf("%d",&id);

    FILE *fp = fopen("books.txt","r");

    if(fp == NULL)
    {
        printf("No Books Found\n");
        return;
    }

    while(fscanf(fp,"%d|%49[^|]|%49[^|]|%d\n",
                 &b.id,b.title,b.author,&b.quantity)==4)
    {
        if(b.id == id)
        {
            printf("\nBook Found\n");
            printf("Title : %s\n",b.title);
            printf("Author: %s\n",b.author);
            printf("Qty   : %d\n",b.quantity);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if(!found)
        printf("Book Not Found\n");
}

void deleteBook()
{
    struct Book b;
    int id, found = 0;

    printf("Enter Book ID to Delete: ");
    scanf("%d",&id);

    FILE *fp = fopen("books.txt","r");
    FILE *temp = fopen("temp.txt","w");

    if(fp == NULL)
    {
        printf("No Books Found\n");
        return;
    }

    while(fscanf(fp,"%d|%49[^|]|%49[^|]|%d\n",
                 &b.id,b.title,b.author,&b.quantity)==4)
    {
        if(b.id == id)
        {
            found = 1;
            continue;
        }

        fprintf(temp,"%d|%s|%s|%d\n",
                b.id,b.title,b.author,b.quantity);
    }

    fclose(fp);
    fclose(temp);

    remove("books.txt");
    rename("temp.txt","books.txt");

    if(found)
        printf("Book Deleted\n");
    else
        printf("Book Not Found\n");
}

/* ---------------- FINE ---------------- */

int fineCalc(int days)
{
    if(days > 7)
        return (days - 7) * 100;

    return 0;
}

/* ---------------- ISSUE BOOK ---------------- */

void issueBook()
{
    struct Transaction t;

    printf("Student ID: ");
    scanf("%d",&t.studentId);

    printf("Book ID: ");
    scanf("%d",&t.bookId);

    printf("Issue Day: ");
    scanf("%d",&t.issueDay);

    t.returnDay = 0;
    t.fine = 0;
    t.returned = 0;

    FILE *fp = fopen("transactions.txt","a");

    if(fp == NULL)
    {
        printf("File Error\n");
        return;
    }

    fprintf(fp,"%d|%d|%d|%d|%d|%d\n",
            t.studentId,
            t.bookId,
            t.issueDay,
            t.returnDay,
            t.fine,
            t.returned);

    fclose(fp);

    printf("Book Issued\n");
}

/* ---------------- RETURN BOOK ---------------- */

void returnBook()
{
    struct Transaction t;
    int sid,bid,day,found=0;

    printf("Student ID: ");
    scanf("%d",&sid);

    printf("Book ID: ");
    scanf("%d",&bid);

    printf("Return Day: ");
    scanf("%d",&day);

    FILE *fp = fopen("transactions.txt","r");
    FILE *temp = fopen("tempTrans.txt","w");

    if(fp == NULL)
    {
        printf("No Transactions Found\n");
        return;
    }

    while(fscanf(fp,"%d|%d|%d|%d|%d|%d\n",
                 &t.studentId,
                 &t.bookId,
                 &t.issueDay,
                 &t.returnDay,
                 &t.fine,
                 &t.returned)==6)
    {
        if(t.studentId==sid &&
           t.bookId==bid &&
           t.returned==0)
        {
            t.returnDay = day;
            t.fine = fineCalc(day - t.issueDay);
            t.returned = 1;
            found = 1;

            printf("Returned Successfully\n");
            printf("Fine = %d\n",t.fine);
        }

        fprintf(temp,"%d|%d|%d|%d|%d|%d\n",
                t.studentId,
                t.bookId,
                t.issueDay,
                t.returnDay,
                t.fine,
                t.returned);
    }

    fclose(fp);
    fclose(temp);

    remove("transactions.txt");
    rename("tempTrans.txt","transactions.txt");

    if(!found)
        printf("Record Not Found\n");
}

/* ---------------- VIEW TRANSACTIONS ---------------- */

void viewTransactions()
{
    struct Transaction t;

    FILE *fp = fopen("transactions.txt","r");

    if(fp == NULL)
    {
        printf("No Transactions\n");
        return;
    }

    printf("\nSID\tBID\tIssue\tReturn\tFine\tStatus\n");

    while(fscanf(fp,"%d|%d|%d|%d|%d|%d\n",
                 &t.studentId,
                 &t.bookId,
                 &t.issueDay,
                 &t.returnDay,
                 &t.fine,
                 &t.returned)==6)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%s\n",
               t.studentId,
               t.bookId,
               t.issueDay,
               t.returnDay,
               t.fine,
               t.returned ? "Returned" : "Issued");
    }

    fclose(fp);
}

/* ---------------- MENUS ---------------- */

void adminMenu()
{
    int ch;

    do
    {
        printf("\nADMIN MENU\n");
        printf("1.Add Book\n");
        printf("2.View Books\n");
        printf("3.Search Book\n");
        printf("4.Delete Book\n");
        printf("5.View Transactions\n");
        printf("6.Exit\n");

        scanf("%d",&ch);

        switch(ch)
        {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: viewTransactions(); break;
        }

    }while(ch!=6);
}

void librarianMenu()
{
    int ch;

    do
    {
        printf("\nLIBRARIAN MENU\n");
        printf("1.Issue Book\n");
        printf("2.Return Book\n");
        printf("3.Search Book\n");
        printf("4.View Books\n");
        printf("5.Exit\n");

        scanf("%d",&ch);

        switch(ch)
        {
            case 1: issueBook(); break;
            case 2: returnBook(); break;
            case 3: searchBook(); break;
            case 4: viewBooks(); break;
        }

    }while(ch!=5);
}

void studentMenu()
{
    int ch;

    do
    {
        printf("\nSTUDENT MENU\n");
        printf("1.View Books\n");
        printf("2.Search Book\n");
        printf("3.Exit\n");

        scanf("%d",&ch);

        switch(ch)
        {
            case 1: viewBooks(); break;
            case 2: searchBook(); break;
        }

    }while(ch!=3);
}

/* ---------------- MAIN ---------------- */

int main()
{
    int ch;
    char u[20], p[20];

    while(1)
    {
        printf("\n1.Admin\n2.Librarian\n3.Student\n4.Exit\n");
        scanf("%d",&ch);

        switch(ch)
        {
            case 1:
                printf("User: ");
                scanf("%s",u);

                printf("Pass: ");
                scanf("%s",p);

                if(strcmp(u,"admin")==0 &&
                   strcmp(p,"123")==0)
                    adminMenu();
                else
                    printf("Wrong Username/Password\n");
                break;

            case 2:
                printf("User: ");
                scanf("%s",u);

                printf("Pass: ");
                scanf("%s",p);

                if(strcmp(u,"lib")==0 &&
                   strcmp(p,"123")==0)
                    librarianMenu();
                else
                    printf("Wrong Username/Password\n");
                break;

            case 3:
                studentMenu();
                break;

            case 4:
                exit(0);
        }
    }

    return 0;
}

