#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>

typedef struct Contact {
    char name[50];
    char email[50];
    char phone[20];
    int id;             
    struct Contact* left;
    struct Contact* right;
} Contact;

Contact* createContact(char* name, char* email, char* phone);
Contact* insert(Contact* root, char* name, char* email, char* phone);
int isValidPhone(char* phone);

Contact* searchByName(Contact* root, char* name);
Contact* searchByPhone(Contact* root, char* phone);
Contact* searchByEmail(Contact* root, char* email);

Contact* deleteByName(Contact* root, char* name);
Contact* deleteByPhone(Contact* root, char* phone);
Contact* deleteByEmail(Contact* root, char* email);
Contact* findMin(Contact* root);


void inorder(Contact* root);
void displayByLetter(Contact* root, char ch);
void displayInsertion(Contact* root);

void saveContacts(Contact* root, FILE* fp);
void saveToFile(Contact* root);
Contact* loadFromFile(Contact* root);
void editContact(Contact* root);

#endif
