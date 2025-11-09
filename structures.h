#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---------- USER LINKED LIST ---------- */
typedef struct User {
    char username[50];
    char password[50];
    struct User *next;
} User;

/* ---------- EVENT LINKED LIST ---------- */
typedef struct Event {
    int id;
    char name[80];
    char date[16]; // DD/MM/YYYY
    char time[8];  // HH:MM
    int capacity;
    int price;
    struct Event *next;
} Event;

/* ---------- BOOKING LINKED LIST ---------- */
typedef struct Booking {
    char bookingId[20];
    char username[50];
    char eventName[80];
    int eventId;
    char phone[20];
    char status[20];
    char paymentId[20];
    struct Booking *next;
} Booking;

/* ---------- PAYMENT RECORD ---------- */
typedef struct Payment {
    char paymentId[20];
    char username[50];
    int eventId;
    char mode[20];
    int amount;
    char status[20];
} Payment;

/* ---------- NOTIFICATION LINKED LIST ---------- */
typedef struct Notification {
    char username[50];
    char message[256];
    struct Notification *next;
} Notification;

/* ---------- WAITING QUEUE NODE ---------- */
typedef struct WaitNode {
    char username[50];
    char phone[20];
    int eventId;
    struct WaitNode *next;
} WaitNode;

/* ---------- BST NODE for booking search ---------- */
typedef struct BSTNode {
    char bookingId[20];
    char username[50];
    int eventId;
    char status[20];
    struct BSTNode *left, *right;
} BSTNode;

/* ---------- Function prototypes ---------- */

/* utility */
void ensure_files_exist();

/* user.c */
void load_users_from_file();
void save_users_to_file();
int register_user();
int login_user(char *out_username);

/* event.c */
void load_events_from_file();
void save_events_to_file();
void add_event_interactive();
void view_events();
int get_event_name_and_price(int id, char *out_name, int *out_price);
void edit_event_interactive();
void delete_event_interactive();

/* booking.c */
void load_bookings_from_file();
void save_bookings_to_file();
void book_ticket_flow(const char *username);
void cancel_booking_flow(const char *username);
void view_bookings_for_user(const char *username);

/* payment.c */
void process_payment_record(const char *username, const char *bookingId, int amount, char *out_paymentId);
void view_payments_for_user(const char *username);

/* notification.c */
void load_notifications_from_file();
void save_notifications_to_file();
void add_notification(const char *username, const char *message);
void view_notifications(const char *username);

/* waiting.c */
void load_waiting_from_file();
void save_waiting_to_file();
void enqueue_waiting(const char *username, const char *phone, int eventId);
WaitNode* dequeue_waiting_for_event(int eventId);

/* bst.c */
BSTNode* bst_insert(BSTNode *root, const char *bookingId, const char *username, int eventId, const char *status);
BSTNode* bst_search(BSTNode *root, const char *bookingId);
void bst_free(BSTNode *root);
void build_bst_from_bookings();

/* feedback.c */
void add_feedback(const char *username, const char *message);
void view_feedback();

#endif

