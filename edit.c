#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

extern void speak(const char* text);

void editContact(Contact* root) {
    if(!root) return;

    char searchName[50];
    printf("Enter Name of contact to edit: ");
    fgets(searchName, 50, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    Contact* curr = root;
    while(curr != NULL) {
        if(strcmp(curr->name, searchName) == 0) break;
        if(strcmp(searchName, curr->name) < 0) curr = curr->left;
        else curr = curr->right;
    }

    if(!curr) {
        printf("\nContact not found!\n");
        speak("Contact not found!");
        return;
    }

    printf("Editing Contact: %s\nPhone: %s\nEmail: %s\n",
           curr->name, curr->phone, curr->email);

    char newName[50], newPhone[20], newEmail[50];

    printf("Enter New Name (leave blank to keep same): ");
    fgets(newName, 50, stdin);
    newName[strcspn(newName, "\n")] = '\0';
    if(strlen(newName) > 0) strcpy(curr->name, newName);

    printf("Enter New Phone (10 digits, leave blank to keep same): ");
    fgets(newPhone, 20, stdin);
    newPhone[strcspn(newPhone, "\n")] = '\0';
    if(strlen(newPhone) > 0 && isValidPhone(newPhone))
        strcpy(curr->phone, newPhone);

    printf("Enter New Email (leave blank to keep same): ");
    fgets(newEmail, 50, stdin);
    newEmail[strcspn(newEmail, "\n")] = '\0';
    if(strlen(newEmail) > 0) strcpy(curr->email, newEmail);

    printf("Contact updated!\n");
    char msg[120];
    sprintf(msg, "Contact %s updated successfully!", curr->name);
    speak(msg);
}

void saveContacts(Contact* root, FILE* fp) {
    if(!root) return;
    saveContacts(root->left, fp);
    fprintf(fp, "Name : %s\nPhonenumber: %s\nEmail: %s\n\n", root->name, root->phone, root->email);
    saveContacts(root->right, fp);
}

void saveToFile(Contact* root) {
    FILE* fp = fopen("contacts.txt", "w");
    if(!fp) {
        printf("Error opening file!\n");
        speak("Error opening file!");
        return;
    }
    saveContacts(root, fp);
    fclose(fp);
    printf("Contacts saved to file!\n");
    speak("Contacts saved successfully!");
}

Contact* loadFromFile(Contact* root) {
    FILE* fp = fopen("contacts.txt", "r");
    if (!fp) {
        printf("No saved contacts found.\n");
        speak("No saved contacts found!");
        return root;
    }

    char name[50], phone[20], email[50];
    while (fscanf(fp, "%s %s %s", name, phone, email) == 3) {
        root = insert(root, name, email, phone);
    }

    fclose(fp);
    printf("Contacts loaded from file!\n");
    speak("Contacts loaded successfully!");
    return root;
}
