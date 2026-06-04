#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "contact.h"

extern void speak(const char* text);

static void make_safe_copy(const char* src, char* dst, int dstSize) {
    int j = 0;
    for (int i = 0; src[i] != '\0' && j < dstSize - 1; i++) {
        char c = src[i];
        if (c == '\'') { 
            dst[j++] = ' ';
        } else {
            dst[j++] = c;
        }
    }
    dst[j] = '\0';
}

static void speak_contact_name(const char* name) {
    char safeName[128];
    make_safe_copy(name, safeName, sizeof(safeName));
    char msg[200];
    snprintf(msg, sizeof(msg), "Contact %s", safeName);
    speak(msg);
}

void inorder(Contact* root) {
    if (!root) return;
    inorder(root->left);

    printf("\nName: %s\nPhone: %s\nEmail: %s\n", root->name, root->phone, root->email);
    speak_contact_name(root->name);

    inorder(root->right);
}

void displayByLetter(Contact* root, char ch) {
    if (!root) return;
    displayByLetter(root->left, ch);
    if (root->name[0] != '\0' && toupper(root->name[0]) == toupper(ch)) {
        printf("\nName: %s\nPhone: %s\nEmail: %s\n", root->name, root->phone, root->email);
        speak_contact_name(root->name);
    }
    displayByLetter(root->right, ch);
}

void collectAllContacts(Contact* root, Contact* arr[], int* idx) {
    if (!root) return;
    arr[(*idx)++] = root;
    collectAllContacts(root->left, arr, idx);
    collectAllContacts(root->right, arr, idx);
}

void displayInsertion(Contact* root) {
    if (!root) {
        printf("No contacts!\n");
        return;
    }

    Contact* arr[1000];
    int count = 0;
    collectAllContacts(root, arr, &count);
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (arr[i]->id > arr[j]->id) {
                Contact* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    printf("\n--- Contacts (Insertion Order) ---\n");
    for (int i = 0; i < count; i++) {
        printf("\nName: %s\nPhone: %s\nEmail: %s\n", arr[i]->name, arr[i]->phone, arr[i]->email);
        speak_contact_name(arr[i]->name);
    }
}
