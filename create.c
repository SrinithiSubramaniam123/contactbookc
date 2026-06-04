#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

extern void speak(const char* text);

Contact* createContact(char* name, char* email, char* phone) {
    Contact* newNode = (Contact*)malloc(sizeof(Contact));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    strcpy(newNode->name, name);
    strcpy(newNode->email, email);
    strcpy(newNode->phone, phone);
    newNode->left = newNode->right = NULL;

    return newNode;
}

Contact* insert(Contact* root, char* name, char* email, char* phone) {
    if (!root) {
        Contact* newC = createContact(name, email, phone);
        return newC;
    }

    if (strcmp(name, root->name) < 0)
        root->left = insert(root->left, name, email, phone);
    else if (strcmp(name, root->name) > 0)
        root->right = insert(root->right, name, email, phone);
    else {
        printf("Duplicate contact name found: %s\n", name);
    }

    return root;
}

int isValidPhone(char* phone) {
    if (strlen(phone) != 10) {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return 0;
        }
    }

    return 1;
}
