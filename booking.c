#include "structures.h"

extern Booking *bookings_head;
extern Event *events_head;

void load_bookings_from_file() {
    bookings_head = NULL;
    FILE *fp = fopen("files/bookings.txt","r");
    if (!fp) return;
    char bid[20], user[50], ename[80], phone[20], status[20], pid[20];
    int eid;
    while (fscanf(fp,"%19s %49s %79s %d %19s %19s %19s", bid, user, ename, &eid, phone, status, pid) == 7) {
        Booking *n = (Booking*)malloc(sizeof(Booking));
        strcpy(n->bookingId, bid); strcpy(n->username, user); strcpy(n->eventName, ename);
        n->eventId = eid; strcpy(n->phone, phone); strcpy(n->status, status); strcpy(n->paymentId, pid);
        n->next = bookings_head; bookings_head = n;
    }
    fclose(fp);
}

void save_bookings_to_file() {
    FILE *fp = fopen("files/bookings.txt","w");
    if (!fp) return;
    Booking *t = bookings_head;
    while (t) {
        fprintf(fp,"%s %s %s %d %s %s %s\n", t->bookingId, t->username, t->eventName, t->eventId, t->phone, t->status, t->paymentId);
        t = t->next;
    }
    fclose(fp);
}

void book_ticket_flow(const char *username) {
    view_events();
    int eid; printf("Enter Event ID to book: "); scanf("%d", &eid);
    char ename[80]; int price;
    if (!get_event_name_and_price(eid, ename, &price)) { printf("Invalid event.\n"); return; }
    // find event node to check capacity
    Event *e = events_head; while (e && e->id!=eid) e = e->next;
    if (!e) { printf("Event missing.\n"); return; }
    if (e->capacity <= 0) {
        printf("Event full. Adding to waiting list.\n");
        char phone[20]; printf("Enter phone: "); scanf("%19s", phone);
        enqueue_waiting(username, phone, eid);
        add_notification(username, "Added to waiting list.");
        save_waiting_to_file();
        return;
    }
    char phone[20]; printf("Enter your phone: "); scanf("%19s", phone);
    Booking *n = (Booking*)malloc(sizeof(Booking));
    snprintf(n->bookingId, sizeof(n->bookingId), "B%d", rand()%90000 + 1000);
    strcpy(n->username, username); strcpy(n->eventName, ename); n->eventId = eid;
    strcpy(n->phone, phone); strcpy(n->status, "Confirmed"); n->next = bookings_head; bookings_head = n;
    e->capacity -= 1; save_events_to_file();
    char paymentId[20];
    process_payment_record(username, n->bookingId, price, paymentId);
    strcpy(n->paymentId, paymentId);
    save_bookings_to_file();
    char msg[256]; snprintf(msg, sizeof(msg), "Booking confirmed for %s (ID:%d). Booking:%s Payment:%s", ename, eid, n->bookingId, n->paymentId);
    add_notification(username, msg);
    printf("Booking done. ID: %s\n", n->bookingId);
}

void cancel_booking_flow(const char *username) {
    view_bookings_for_user(username);
    char bid[20]; printf("Enter BookingID to cancel: "); scanf("%19s", bid);
    Booking *t = bookings_head, *prev = NULL;
    while (t) { if (strcmp(t->bookingId,bid)==0 && strcmp(t->username, username)==0) break; prev = t; t = t->next; }
    if (!t) { printf("Not found or not yours.\n"); return; }
    strcpy(t->status, "Cancelled");
    // increase event capacity
    Event *e = events_head; while (e && e->id!=t->eventId) e = e->next;
    if (e) { e->capacity += 1; save_events_to_file(); }
    save_bookings_to_file();
    add_notification(username, "Your booking was cancelled.");
    // promote waiting
    WaitNode *w = dequeue_waiting_for_event(t->eventId);
    if (w) {
        Booking *nb = (Booking*)malloc(sizeof(Booking));
        snprintf(nb->bookingId, sizeof(nb->bookingId), "B%d", rand()%90000 + 1000);
        strcpy(nb->username, w->username);
        char en[80]; int pr; get_event_name_and_price(t->eventId, en, &pr);
        strcpy(nb->eventName, en); nb->eventId = t->eventId; strcpy(nb->phone, w->phone); strcpy(nb->status, "Confirmed");
        nb->next = bookings_head; bookings_head = nb;
        Event *ev = events_head; while (ev && ev->id!=t->eventId) ev = ev->next;
        if (ev) { ev->capacity -= 1; save_events_to_file(); }
        save_bookings_to_file(); save_waiting_to_file();
        add_notification(nb->username, "A seat became available and your booking is confirmed.");
        free(w);
    }
}

void view_bookings_for_user(const char *username) {
    Booking *t = bookings_head; int found = 0;
    printf("\n--- Your Bookings ---\n");
    while (t) {
        if (strcmp(t->username, username)==0) {
            printf("%s | %s (%d) | %s | %s | %s\n", t->bookingId, t->eventName, t->eventId, t->phone, t->status, t->paymentId);
            found = 1;
        }
        t = t->next;
    }
    if (!found) printf("No bookings.\n");
}

