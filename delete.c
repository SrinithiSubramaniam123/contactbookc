#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

extern void speak(const char* text);

Contact* findMin(Contact* root) {
    Contact* current = root;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

Contact* deleteByName(Contact* root, char name[]) {
    if (!root) return NULL;

    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        root->left = deleteByName(root->left, name);
    } else if (cmp > 0) {
        root->right = deleteByName(root->right, name);
    } else {
        char msg[120];
        sprintf(msg, "Contact %s deleted successfully!", name);
        speak(msg);

        if (!root->left) {
            Contact* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            Contact* temp = root->left;
            free(root);
            return temp;
        }

        Contact* temp = findMin(root->right);
        strcpy(root->name, temp->name);
        strcpy(root->email, temp->email);
        strcpy(root->phone, temp->phone);
        root->right = deleteByName(root->right, temp->name);
    }
    return root;
}

Contact* deleteByEmail(Contact* root, char email[]) {
    Contact* target = searchByEmail(root, email);
    if (!target) return root;
    return deleteByName(root, target->name);
}

Contact* deleteByPhone(Contact* root, char phone[]) {
    Contact* target = searchByPhone(root, phone);
    if (!target) return root;
    return deleteByName(root, target->name);
}
