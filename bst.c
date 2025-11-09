#include "structures.h"

extern Booking *bookings_head;
BSTNode *bst_root = NULL;

BSTNode* bst_insert(BSTNode *root, const char *bookingId, const char *username, int eventId, const char *status) {
    if (!root) {
        BSTNode *n = (BSTNode*)malloc(sizeof(BSTNode));
        strcpy(n->bookingId, bookingId); strcpy(n->username, username); n->eventId = eventId; strcpy(n->status, status);
        n->left = n->right = NULL; return n;
    }
    int cmp = strcmp(bookingId, root->bookingId);
    if (cmp < 0) root->left = bst_insert(root->left, bookingId, username, eventId, status);
    else if (cmp > 0) root->right = bst_insert(root->right, bookingId, username, eventId, status);
    return root;
}

BSTNode* bst_search(BSTNode *root, const char *bookingId) {
    if (!root) return NULL;
    int cmp = strcmp(bookingId, root->bookingId);
    if (cmp == 0) return root;
    if (cmp < 0) return bst_search(root->left, bookingId);
    return bst_search(root->right, bookingId);
}

void bst_free(BSTNode *root) {
    if (!root) return;
    bst_free(root->left); bst_free(root->right); free(root);
}

void build_bst_from_bookings() {
    bst_free(bst_root); bst_root = NULL;
    Booking *t = bookings_head;
    while (t) {
        bst_root = bst_insert(bst_root, t->bookingId, t->username, t->eventId, t->status);
        t = t->next;
    }
}

