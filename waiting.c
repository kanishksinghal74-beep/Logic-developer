#include "structures.h"

extern WaitNode *waiting_head;
extern WaitNode *waiting_tail;

void load_waiting_from_file() {
    waiting_head = waiting_tail = NULL;
    FILE *fp = fopen("files/waiting.txt","r");
    if (!fp) return;
    char user[50], phone[20]; int eid;
    while (fscanf(fp,"%49s %19s %d", user, phone, &eid) == 3) {
        WaitNode *n = (WaitNode*)malloc(sizeof(WaitNode));
        strcpy(n->username, user); strcpy(n->phone, phone); n->eventId = eid; n->next = NULL;
        if (!waiting_head) waiting_head = waiting_tail = n; else { waiting_tail->next = n; waiting_tail = n; }
    }
    fclose(fp);
}

void save_waiting_to_file() {
    FILE *fp = fopen("files/waiting.txt","w");
    if (!fp) return;
    WaitNode *t = waiting_head;
    while (t) { fprintf(fp, "%s %s %d\n", t->username, t->phone, t->eventId); t = t->next; }
    fclose(fp);
}

void enqueue_waiting(const char *username, const char *phone, int eventId) {
    WaitNode *n = (WaitNode*)malloc(sizeof(WaitNode));
    strcpy(n->username, username); strcpy(n->phone, phone); n->eventId = eventId; n->next = NULL;
    if (!waiting_head) waiting_head = waiting_tail = n; else { waiting_tail->next = n; waiting_tail = n; }
    FILE *fp = fopen("files/waiting.txt","a"); if (fp) { fprintf(fp, "%s %s %d\n", username, phone, eventId); fclose(fp); }
}

WaitNode* dequeue_waiting_for_event(int eventId) {
    WaitNode *prev = NULL, *cur = waiting_head;
    while (cur) {
        if (cur->eventId == eventId) {
            if (!prev) waiting_head = cur->next; else prev->next = cur->next;
            if (cur == waiting_tail) waiting_tail = prev;
            cur->next = NULL;
            save_waiting_to_file();
            return cur;
        }
        prev = cur; cur = cur->next;
    }
    return NULL;
}

