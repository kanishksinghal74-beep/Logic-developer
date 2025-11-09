#include "structures.h"

extern Event *events_head;

void load_events_from_file() {
    events_head = NULL;
    FILE *fp = fopen("files/events.txt","r");
    if (!fp) return;
    int id, cap, price;
    char name[80], date[16], time[8];
    while (fscanf(fp,"%d %79s %15s %7s %d %d", &id, name, date, time, &cap, &price) == 6) {
        Event *n = (Event*)malloc(sizeof(Event));
        n->id = id; strcpy(n->name, name); strcpy(n->date, date); strcpy(n->time, time);
        n->capacity = cap; n->price = price;
        n->next = events_head; events_head = n;
    }
    fclose(fp);
}

void save_events_to_file() {
    FILE *fp = fopen("files/events.txt","w");
    if (!fp) return;
    Event *t = events_head;
    while (t) {
        fprintf(fp,"%d %s %s %s %d %d\n", t->id, t->name, t->date, t->time, t->capacity, t->price);
        t = t->next;
    }
    fclose(fp);
}

void add_event_interactive() {
    int id, cap, price;
    char name[80], date[16], time[8];
    printf("Enter Event ID: "); scanf("%d", &id);
    Event *x = events_head; while (x) { if (x->id==id) { printf("ID exists.\n"); return; } x = x->next; }
    printf("Event Name (no spaces): "); scanf("%79s", name);
    printf("Date (DD/MM/YYYY): "); scanf("%15s", date);
    printf("Time (HH:MM): "); scanf("%7s", time);
    printf("Capacity: "); scanf("%d", &cap);
    printf("Price: "); scanf("%d", &price);
    Event *n = (Event*)malloc(sizeof(Event));
    n->id = id; strcpy(n->name,name); strcpy(n->date,date); strcpy(n->time,time); n->capacity = cap; n->price = price;
    n->next = events_head; events_head = n;
    save_events_to_file();
    printf("Event added.\n");
}

void view_events() {
    if (!events_head) { printf("No events.\n"); return; }
    Event *t = events_head;
    printf("\n--- Events ---\n");
    while (t) {
        printf("ID:%d | %s | %s %s | Cap:%d | ₹%d\n", t->id, t->name, t->date, t->time, t->capacity, t->price);
        t = t->next;
    }
}

int get_event_name_and_price(int id, char *out_name, int *out_price) {
    Event *t = events_head;
    while (t) {
        if (t->id == id) {
            if (out_name) strcpy(out_name, t->name);
            if (out_price) *out_price = t->price;
            return 1;
        }
        t = t->next;
    }
    return 0;
}

void edit_event_interactive() {
    int id; printf("Enter Event ID to edit: "); scanf("%d", &id);
    Event *t = events_head;
    while (t && t->id!=id) t = t->next;
    if (!t) { printf("Not found.\n"); return; }
    printf("New name: "); scanf("%79s", t->name);
    printf("New date: "); scanf("%15s", t->date);
    printf("New time: "); scanf("%7s", t->time);
    printf("New capacity: "); scanf("%d", &t->capacity);
    printf("New price: "); scanf("%d", &t->price);
    save_events_to_file();
    printf("Updated.\n");
}

void delete_event_interactive() {
    int id; printf("Enter Event ID to delete: "); scanf("%d", &id);
    Event *t = events_head, *prev = NULL;
    while (t && t->id!=id) { prev = t; t = t->next; }
    if (!t) { printf("Not found.\n"); return; }
    if (!prev) events_head = t->next; else prev->next = t->next;
    free(t);
    save_events_to_file();
    printf("Deleted.\n");
}

