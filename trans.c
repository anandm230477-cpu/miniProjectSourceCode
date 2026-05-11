// Bank Account Management System using Random Access Files

#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void createFile();
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayRecords(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // Create file if it doesn't exist
    createFile();

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 6)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            displayRecords(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("Program terminated.\n");

    return 0;
}

// create empty file with 100 blank records
void createFile()
{
    FILE *fPtr;

    struct clientData blankClient = {0, "", "", 0.0};

    if ((fPtr = fopen("credit.dat", "rb")) == NULL)
    {
        fPtr = fopen("credit.dat", "wb");

        for (int i = 0; i < SIZE; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        }

        fclose(fPtr);
    }
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n====== BANK MANAGEMENT SYSTEM ======\n");
    printf("1 - Store formatted text file\n");
    printf("2 - Update an account\n");
    printf("3 - Add a new account\n");
    printf("4 - Delete an account\n");
    printf("5 - Display all accounts\n");
    printf("6 - Exit\n");
    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}

// create formatted text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client = {0, "", "", 0.0};

    rewind(readPtr);

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be created.\n");
        return;
    }

    fprintf(writePtr, "%-6s%-15s%-15s%-10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-15s%-15s%-10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt created successfully.\n");
}

// add new record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter Last Name First Name Balance:\n");
        scanf("%14s %9s %lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account added successfully.\n");
    }
}

// update account
void updateRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int account;
    double transaction;

    printf("Enter account number to update: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        printf("\nCurrent Record\n");
        printf("%d %s %s %.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        printf("Enter transaction amount (+deposit / -withdraw): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Updated Balance = %.2f\n", client.balance);
    }
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// display all records
void displayRecords(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);

    printf("\n%-6s%-15s%-15s%-10s\n",
           "Acct",
           "Last Name",
           "First Name",
           "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d%-15s%-15s%-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}