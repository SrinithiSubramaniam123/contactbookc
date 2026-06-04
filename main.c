#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  
#include "contact.h"

void speak(const char* text) {
    char safeText[512];
    int j = 0;
    for (int i = 0; text[i] != '\0' && j < (int)sizeof(safeText) - 1; i++) {
        if (text[i] == '\'' || text[i] == '\"') safeText[j++] = ' ';
        else safeText[j++] = text[i];
    }
    safeText[j] = '\0';

    char command[1200];
    snprintf(command, sizeof(command),
        "PowerShell -WindowStyle Hidden -Command \"Add-Type -AssemblyName System.Speech; "
        "$speak = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
        "$speak.Speak('%s');\"",
        safeText);

    system(command);
}

void loginSystem() {
    char username[30], password[30];
    const char correctUser[] = "admin";
    const char correctPass[] = "1234";
    int attempts = 3;

    while (attempts--) {
        printf("\nEnter Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';

        printf("Enter Password: ");
        int i = 0;
        char ch;
        while (1) {
            ch = getch();
            if (ch == 13) { 
                password[i] = '\0';
                break;
            } else if (ch == 8 && i > 0) { 
                i--;
                printf("\b \b");
            } else if (ch != 8) {
                password[i++] = ch;
                printf("*");
            }
        }

        if (strcmp(username, correctUser) == 0 && strcmp(password, correctPass) == 0) {
            printf("\nLogin Successful! Welcome, %s\n", username);
            char msg[200];
            snprintf(msg, sizeof(msg), "Login successful. Welcome %s!", username);
            speak(msg);
            return;
        } else {
            printf("\nIncorrect username or password. Attempts left: %d\n", attempts);
            speak("Incorrect username or password!");
        }
    }

    printf("\nToo many failed attempts! Exiting...\n");
    speak("Too many failed attempts! Exiting!");
    exit(0);
}
int main() {
    loginSystem();

    Contact* root = NULL;
    int choice, subChoice;
    char name[50], email[50], phone[20];
    int contactCount;

    while (1) {
        printf("\n--- Contact Book ---\n");
        printf("\n1. Add Contacts\n");
        printf("2. Search Contact\n");
        printf("3. Delete Contact\n");
        printf("4. Display Contacts\n");
        printf("5. Edit Contact\n");
        printf("6. Save Contacts\n");
        printf("7. Load Contacts\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Try again.\n");
            continue;
        }
        getchar();

        switch (choice) {
            case 1:
                printf("How many contacts you want to save? ");
                if (scanf("%d", &contactCount) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid number.\n"); break; }
                getchar();

                for (int c = 0; c < contactCount;) {
                    printf("\n--- Contact %d ---\n", c + 1);

                    printf("Enter Name: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';

                    if (searchByName(root, name)) {
                        printf("Duplicate name found! Try again.\n");
                        speak("Duplicate name found. Try again.");
                        continue;
                    }

                    while (1) {
                        printf("Enter Phone: ");
                        fgets(phone, sizeof(phone), stdin);
                        phone[strcspn(phone, "\n")] = '\0';

                        if (!isValidPhone(phone)) {
                            printf("Invalid! Phone must be exactly 10 digits.\n");
                            speak("Invalid phone number!");
                            continue;
                        }

                        if (searchByPhone(root, phone)) {
                            printf("Duplicate phone found! Try again.\n");
                            speak("Duplicate phone found. Try again.");
                            continue;
                        }
                        break;
                    }

                    printf("Enter Email: ");
                    fgets(email, sizeof(email), stdin);
                    email[strcspn(email, "\n")] = '\0';

                    if (searchByEmail(root, email)) {
                        printf("Duplicate email found! Try again.\n");
                        speak("Duplicate email found. Try again.");
                        continue;
                    }

                    root = insert(root, name, email, phone);
                    printf("Contact %d added!\n", c + 1);
                    char msg[200];
                    snprintf(msg, sizeof(msg), "Contact %s added successfully!", name);
                    speak(msg);
                    c++;
                }
                break;

            case 2:
                printf("\n-- Search Menu --\n");
                printf("1. By Name\n");
                printf("2. By Phone\n");
                printf("3. By Email\n");
                printf("Enter choice: ");
                if (scanf("%d", &subChoice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid choice.\n"); break; }
                getchar();

                {
                    Contact* result = NULL;
                    switch (subChoice) {
                        case 1:
                            printf("Enter name to search: ");
                            fgets(name, sizeof(name), stdin);
                            name[strcspn(name, "\n")] = '\0';
                            result = searchByName(root, name);
                            break;
                        case 2:
                            printf("Enter phone to search: ");
                            fgets(phone, sizeof(phone), stdin);
                            phone[strcspn(phone, "\n")] = '\0';
                            result = searchByPhone(root, phone);
                            break;
                        case 3:
                            printf("Enter email to search: ");
                            fgets(email, sizeof(email), stdin);
                            email[strcspn(email, "\n")] = '\0';
                            result = searchByEmail(root, email);
                            break;
                        default:
                            printf("Invalid choice!\n");
                            speak("Invalid choice!");
                    }

                    if (result) {
                        printf("\nFound:\nName: %s\nPhone: %s\nEmail: %s\n",
                               result->name, result->phone, result->email);
                        char msg[200];
                        snprintf(msg, sizeof(msg), "Contact %s found successfully!", result->name);
                        speak(msg);
                    } else {
                        printf("\nContact not found.\n");
                        speak("Contact not found!");
                    }
                }
                break;
            case 3:
                printf("\n-- Delete Menu --\n");
                printf("1. By Name\n");
                printf("2. By Phone\n");
                printf("3. By Email\n");
                printf("Enter choice: ");
                if (scanf("%d", &subChoice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid choice.\n"); break; }
                getchar();

                switch (subChoice) {
                    case 1:
                        printf("Enter name to delete: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = '\0';
                        if (searchByName(root, name)) {
                            root = deleteByName(root, name);
                            char msg[200];
                            snprintf(msg, sizeof(msg), "Contact %s deleted successfully!", name);
                            speak(msg);
                            printf("Deleted: %s\n", name);
                        } else {
                            printf("Contact not found!\n");
                            speak("Contact not found!");
                        }
                        break;
                    case 2:
                        printf("Enter phone to delete: ");
                        fgets(phone, sizeof(phone), stdin);
                        phone[strcspn(phone, "\n")] = '\0';
                        if (searchByPhone(root, phone)) {
                            root = deleteByPhone(root, phone);
                            char msg2[200];
                            Contact* t = searchByPhone(root, phone); 
                            speak("Contact deleted successfully by phone number!");
                            printf("Deleted contact with phone: %s\n", phone);
                        } else {
                            printf("Contact not found!\n");
                            speak("Contact not found!");
                        }
                        break;
                    case 3:
                        printf("Enter email to delete: ");
                        fgets(email, sizeof(email), stdin);
                        email[strcspn(email, "\n")] = '\0';
                        if (searchByEmail(root, email)) {
                            root = deleteByEmail(root, email);
                            speak("Contact deleted successfully by email!");
                            printf("Deleted contact with email: %s\n", email);
                        } else {
                            printf("Contact not found!\n");
                            speak("Contact not found!");
                        }
                        break;
                    default:
                        printf("Invalid choice!\n");
                        speak("Invalid choice!");
                }
                break;
            case 4:
                printf("\n-- Display Menu --\n");
                printf("1. Display All (Alphabetical)\n");
                printf("2. Display by First Letter\n");
                printf("3. Display All (Insertion Order)\n");
                printf("Enter choice: ");
                if (scanf("%d", &subChoice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid choice.\n"); break; }
                getchar();

                switch (subChoice) {
                    case 1:
                        inorder(root);
                        speak("Displaying contacts alphabetically.");
                        break;
                    case 2: {
                        char ch;
                        printf("Enter Letter: ");
                        if (scanf(" %c", &ch) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid letter.\n"); break; }
                        getchar();
                        displayByLetter(root, ch);
                        char msg[120];
                        snprintf(msg, sizeof(msg), "Displaying contacts starting with letter %c.", ch);
                        speak(msg);
                        break;
                    }
                    case 3:
                        displayInsertion(root);
                        speak("Displaying contacts in insertion order.");
                        break;
                    default:
                        printf("Invalid choice!\n");
                        speak("Invalid choice!");
                }
                break;
            case 5:
                editContact(root);
                break;
            case 6:
                saveToFile(root);
                break;

            case 7:
                root = loadFromFile(root);
                break;

            case 8:
                printf("Exiting...\n");
                speak("Goodbye! See you soon!");
                exit(0);

            default:
                printf("Invalid choice!\n");
                speak("Invalid choice!");
        }
    }

    return 0;
}
