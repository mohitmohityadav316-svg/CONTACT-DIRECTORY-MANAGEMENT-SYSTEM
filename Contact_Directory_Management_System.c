#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_SIZE 50
#define PHONE_SIZE 20
#define EMAIL_SIZE 60

// Each node stores one contact and a pointer to the next node.
struct Contact
{
    char name[NAME_SIZE];
    char phone[PHONE_SIZE];
    char email[EMAIL_SIZE];
    struct Contact *next;
};

struct Contact *head = NULL;

// Convert a string to lowercase (used for case-insensitive comparison).
void toLowerCase(char str[])
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
        str[i] = (char)tolower((unsigned char)str[i]);
}

// Remove newline from input.
void removeNewline(char str[])
{
    str[strcspn(str, "\n")] = '\0';
}

void readLine(char str[], int size)
{
    fgets(str, size, stdin);
    removeNewline(str);
}

struct Contact *findByPhone(char phone[])
{
    struct Contact *temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->phone, phone) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

struct Contact *findByName(char name[])
{
    struct Contact *temp = head;
    char searchName[NAME_SIZE], currentName[NAME_SIZE];

    strcpy(searchName, name);
    toLowerCase(searchName);

    while (temp != NULL)
    {
        strcpy(currentName, temp->name);
        toLowerCase(currentName);
        if (strcmp(currentName, searchName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int phoneExists(char phone[], struct Contact *skip)
{
    struct Contact *temp = head;
    while (temp != NULL)
    {
        if (temp != skip && strcmp(temp->phone, phone) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

int emailExists(char email[], struct Contact *skip)
{
    struct Contact *temp = head;
    char e1[EMAIL_SIZE], e2[EMAIL_SIZE];

    strcpy(e1, email);
    toLowerCase(e1);

    while (temp != NULL)
    {
        strcpy(e2, temp->email);
        toLowerCase(e2);
        if (temp != skip && strcmp(e1, e2) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

void printContact(struct Contact *c)
{
    printf("\nName  : %s\n", c->name);
    printf("Phone : %s\n", c->phone);
    printf("Email : %s\n", c->email);
    printf("------------------------------\n");
}

// MODULE 1: Add contact at beginning, end, or a given position.
void addContact(void)
{
    struct Contact *newNode, *temp;
    int choice, position, i;

    newNode = (struct Contact *)malloc(sizeof(struct Contact));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter name: ");
    readLine(newNode->name, NAME_SIZE);
    printf("Enter phone: ");
    readLine(newNode->phone, PHONE_SIZE);
    printf("Enter email: ");
    readLine(newNode->email, EMAIL_SIZE);

    if (strlen(newNode->name) == 0 || strlen(newNode->phone) == 0 ||
        strlen(newNode->email) == 0)
    {
        printf("All fields are required.\n");
        free(newNode);
        return;
    }

    if (phoneExists(newNode->phone, NULL) || emailExists(newNode->email, NULL))
    {
        printf("Phone or email already exists. Contact not added.\n");
        free(newNode);
        return;
    }

    newNode->next = NULL;

    printf("\n1. Insert at beginning\n2. Insert at end\n3. Insert at position\n");
    printf("Choose: ");
    scanf("%d", &choice);
    getchar(); // consume newline

    if (choice == 1)
    {
        newNode->next = head;
        head = newNode;
    }
    else if (choice == 2)
    {
        if (head == NULL)
            head = newNode;
        else
        {
            temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newNode;
        }
    }
    else if (choice == 3)
    {
        printf("Enter position (1 = beginning): ");
        scanf("%d", &position);
        getchar();

        if (position <= 1 || head == NULL)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            temp = head;
            for (i = 1; i < position - 1 && temp->next != NULL; i++)
                temp = temp->next;

            newNode->next = temp->next;
            temp->next = newNode;
        }
    }
    else
    {
        printf("Invalid choice.\n");
        free(newNode);
        return;
    }

    printf("Contact added successfully.\n");
}

// MODULE 2: Delete contact using name or phone.
void deleteContact(void)
{
    struct Contact *temp, *target;
    int choice;
    char key[NAME_SIZE];

    if (head == NULL)
    {
        printf("Directory is empty.\n");
        return;
    }

    printf("Delete by: 1. Name  2. Phone\nChoose: ");
    scanf("%d", &choice);
    getchar();

    printf("Enter %s: ", choice == 1 ? "name" : "phone");
    readLine(key, NAME_SIZE);

    if (choice == 1)
        target = findByName(key);
    else if (choice == 2)
        target = findByPhone(key);
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    if (target == NULL)
    {
        printf("Contact not found.\n");
        return;
    }

    if (target == head)
        head = head->next;
    else
    {
        temp = head;
        while (temp->next != target)
            temp = temp->next;
        temp->next = target->next;
    }

    free(target);
    printf("Contact deleted successfully.\n");
}

// MODULE 2: Update contact details.
void updateContact(void)
{
    struct Contact *target;
    char oldPhone[PHONE_SIZE], name[NAME_SIZE];
    char phone[PHONE_SIZE], email[EMAIL_SIZE];

    printf("Enter phone number of contact to update: ");
    readLine(oldPhone, PHONE_SIZE);

    target = findByPhone(oldPhone);
    if (target == NULL)
    {
        printf("Contact not found.\n");
        return;
    }

    printf("Enter new name: ");
    readLine(name, NAME_SIZE);
    printf("Enter new phone: ");
    readLine(phone, PHONE_SIZE);
    printf("Enter new email: ");
    readLine(email, EMAIL_SIZE);

    if (strlen(name) == 0 || strlen(phone) == 0 || strlen(email) == 0)
    {
        printf("All fields are required. Update cancelled.\n");
        return;
    }

    if (phoneExists(phone, target) || emailExists(email, target))
    {
        printf("Phone or email belongs to another contact. Update cancelled.\n");
        return;
    }

    strcpy(target->name, name);
    strcpy(target->phone, phone);
    strcpy(target->email, email);
    printf("Contact updated successfully.\n");
}

// MODULE 3: Search by name, phone, or email (partial search supported).
void searchContact(void)
{
    struct Contact *temp = head;
    int choice, found = 0, i;
    char key[EMAIL_SIZE], name[NAME_SIZE], email[EMAIL_SIZE];

    printf("Search by: 1. Name  2. Phone  3. Email\nChoose: ");
    scanf("%d", &choice);
    getchar();

    printf("Enter search value: ");
    readLine(key, EMAIL_SIZE);

    while (temp != NULL)
    {
        int match = 0;

        if (choice == 1)
        {
            strcpy(name, temp->name);
            for (i = 0; name[i] != '\0'; i++)
                name[i] = (char)tolower((unsigned char)name[i]);
            strcpy(email, key);
            for (i = 0; email[i] != '\0'; i++)
                email[i] = (char)tolower((unsigned char)email[i]);
            if (strstr(name, email) != NULL)
                match = 1;
        }
        else if (choice == 2)
        {
            if (strstr(temp->phone, key) != NULL)
                match = 1;
        }
        else if (choice == 3)
        {
            strcpy(email, temp->email);
            for (i = 0; email[i] != '\0'; i++)
                email[i] = (char)tolower((unsigned char)email[i]);
            strcpy(name, key);
            for (i = 0; name[i] != '\0'; i++)
                name[i] = (char)tolower((unsigned char)name[i]);
            if (strstr(email, name) != NULL)
                match = 1;
        }
        else
        {
            printf("Invalid choice.\n");
            return;
        }

        if (match)
        {
            printContact(temp);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found)
        printf("No matching contact found.\n");
}

// MODULE 4: Display all contacts and total count.
void displayContacts(void)
{
    struct Contact *temp = head;
    int count = 0;

    if (head == NULL)
    {
        printf("Directory is empty.\n");
        return;
    }

    while (temp != NULL)
    {
        printContact(temp);
        count++;
        temp = temp->next;
    }
    printf("Total contacts: %d\n", count);
}

// MODULE 4: Detect duplicate phone numbers or emails.
void duplicateCheck(void)
{
    struct Contact *first = head, *second;
    int found = 0;

    while (first != NULL)
    {
        second = first->next;
        while (second != NULL)
        {
            char e1[EMAIL_SIZE], e2[EMAIL_SIZE];
            strcpy(e1, first->email);
            strcpy(e2, second->email);
            toLowerCase(e1);
            toLowerCase(e2);

            if (strcmp(first->phone, second->phone) == 0 ||
                strcmp(e1, e2) == 0)
            {
                printf("\nPossible duplicate contacts:\n");
                printContact(first);
                printContact(second);
                found = 1;
            }
            second = second->next;
        }
        first = first->next;
    }

    if (!found)
        printf("No duplicate contacts found.\n");
}

// MODULE 4: Display contacts alphabetically using bubble sort.
void displayAlphabetically(void)
{
    struct Contact *temp = head;
    struct Contact **arr;
    int count = 0, i, j;
    struct Contact *swap;
    char name1[NAME_SIZE], name2[NAME_SIZE];

    if (head == NULL)
    {
        printf("Directory is empty.\n");
        return;
    }

    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }

    arr = (struct Contact **)malloc(count * sizeof(struct Contact *));
    if (arr == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    temp = head;
    for (i = 0; i < count; i++)
    {
        arr[i] = temp;
        temp = temp->next;
    }

    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - i - 1; j++)
        {
            strcpy(name1, arr[j]->name);
            strcpy(name2, arr[j + 1]->name);
            toLowerCase(name1);
            toLowerCase(name2);

            if (strcmp(name1, name2) > 0)
            {
                swap = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = swap;
            }
        }
    }

    printf("\nContacts in alphabetical order:\n");
    for (i = 0; i < count; i++)
        printContact(arr[i]);

    free(arr);
}

// Free all linked-list nodes before program ends.
void freeDirectory(void)
{
    struct Contact *temp = head;
    while (temp != NULL)
    {
        struct Contact *nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }
    head = NULL;
}

int main(void)
{
    int choice;

    do
    {
        printf("\n========== CONTACT DIRECTORY ==========");
        printf("\n1. Add / Insert Contact");
        printf("\n2. Delete Contact");
        printf("\n3. Update Contact");
        printf("\n4. Search Contact");
        printf("\n5. Display All Contacts and Count");
        printf("\n6. Check Duplicate Contacts");
        printf("\n7. Display Contacts Alphabetically");
        printf("\n0. Exit");
        printf("\nEnter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            addContact();
            break;
        case 2:
            deleteContact();
            break;
        case 3:
            updateContact();
            break;
        case 4:
            searchContact();
            break;
        case 5:
            displayContacts();
            break;
        case 6:
            duplicateCheck();
            break;
        case 7:
            displayAlphabetically();
            break;
        case 0:
            printf("Thank you for using Contact Directory!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    freeDirectory();
    return 0;
}
