#include <stdio.h>
#include <string.h>
#include "contact.h"
extern void speak(const char* text);

Contact* searchByName(Contact* root, char* name) {
    if (!root) return NULL;

    int cmp = strcmp(name, root->name);
    if (cmp == 0) {
        printf("\nContact found: %s\n", root->name);
        return root;
    }

    if (cmp < 0)
        return searchByName(root->left, name);
    else
        return searchByName(root->right, name);
}

Contact* searchByEmail(Contact* root, char* email) {
    if (!root) return NULL;

    if (strcmp(email, root->email) == 0) {
        printf("\nContact found: %s\n", root->name);
        return root;
    }

    Contact* leftResult = searchByEmail(root->left, email);
    if (leftResult) return leftResult;
    return searchByEmail(root->right, email);
}

Contact* searchByPhone(Contact* root, char* phone) {
    if (!root) return NULL;

    if (strcmp(phone, root->phone) == 0) {
        printf("\nContact found: %s\n", root->name);
        return root;
    }

    Contact* leftResult = searchByPhone(root->left, phone);
    if (leftResult) return leftResult;
    return searchByPhone(root->right, phone);
}
