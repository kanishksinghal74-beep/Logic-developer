#include "structures.h"

extern Notification *notifications_head;

void load_notifications_from_file() {
    notifications_head = NULL;
    FILE *fp = fopen("files/notifications.txt","r");
    if (!fp) return;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;
        *colon = '\0';
        char *user = line;
        char *msg = colon+1;
        while (*msg == ' ') msg++;
        char *nl = strchr(msg,'\n'); if (nl) *nl = '\0';
        Notification *n = (Notification*)malloc(sizeof(Notification));
        strcpy(n->username, user); strncpy(n->message, msg, sizeof(n->message)-1); n->message[sizeof(n->message)-1]=0;
        n->next = notifications_head; notifications_head = n;
    }
    fclose(fp);
}

void save_notifications_to_file() {
    FILE *fp = fopen("files/notifications.txt","w");
    if (!fp) return;
    Notification *t = notifications_head;
    while (t) {
        fprintf(fp, "%s: %s\n", t->username, t->message);
        t = t->next;
    }
    fclose(fp);
}

void add_notification(const char *username, const char *message) {
    Notification *n = (Notification*)malloc(sizeof(Notification));
    strcpy(n->username, username); strncpy(n->message, message, sizeof(n->message)-1); n->message[sizeof(n->message)-1]=0;
    n->next = notifications_head; notifications_head = n;
    FILE *fp = fopen("files/notifications.txt","a");
    if (fp) { fprintf(fp, "%s: %s\n", username, message); fclose(fp); }
    printf("Notification added.\n");
}

void view_notifications(const char *username) {
    Notification *t = notifications_head; int found=0;
    printf("\n--- Notifications for %s ---\n", username);
    while (t) {
        if (strcmp(t->username, username)==0) { printf("%s\n", t->message); found=1; }
        t = t->next;
    }
    if (!found) printf("No notifications.\n");
}

