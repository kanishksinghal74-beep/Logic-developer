#include "structures.h"

extern User *users_head;

void load_users_from_file() {
    users_head = NULL;
    FILE *fp = fopen("files/users.txt","r");
    if (!fp) return;
    char u[50], p[50];
    while (fscanf(fp, "%49s %49s", u, p) == 2) {
        User *n = (User*)malloc(sizeof(User));
        strcpy(n->username, u);
        strcpy(n->password, p);
        n->next = users_head;
        users_head = n;
    }
    fclose(fp);
}

void save_users_to_file() {
    FILE *fp = fopen("files/users.txt","w");
    if (!fp) return;
    User *t = users_head;
    while (t) {
        fprintf(fp, "%s %s\n", t->username, t->password);
        t = t->next;
    }
    fclose(fp);
}

int register_user() {
    char u[50], p[50];
    printf("Enter new username: "); scanf("%49s", u);
    printf("Enter password: "); scanf("%49s", p);
    User *t = users_head;
    while (t) { if (strcmp(t->username,u)==0) { printf("User exists.\n"); return 0; } t = t->next; }
    User *n = (User*)malloc(sizeof(User));
    strcpy(n->username,u); strcpy(n->password,p);
    n->next = users_head; users_head = n;
    save_users_to_file();
    printf("Registered.\n");
    return 1;
}

int login_user(char *out_username) {
    char u[50], p[50];
    printf("Username: "); scanf("%49s", u);
    printf("Password: "); scanf("%49s", p);
    User *t = users_head;
    while (t) {
        if (strcmp(t->username,u)==0 && strcmp(t->password,p)==0) {
            if (out_username) strcpy(out_username, u);
            return 1;
        }
        t = t->next;
    }
    return 0;
}

