#include "structures.h"

/* Global heads */
User *users_head = NULL;
Event *events_head = NULL;
Booking *bookings_head = NULL;
Notification *notifications_head = NULL;
WaitNode *waiting_head = NULL;
WaitNode *waiting_tail = NULL;

/* bst root (in bst.c) */
extern BSTNode *bst_root;

/* function prototypes (modules) */
void ensure_files_exist() {
    FILE *f;
    const char *files[] = {
        "files/users.txt", "files/events.txt", "files/bookings.txt",
        "files/payments.txt", "files/notifications.txt",
        "files/waiting.txt", "files/feedback.txt"
    };
    for (int i = 0; i < 7; i++) {
        f = fopen(files[i], "a");
        if (f) fclose(f);
    }
}

/* module functions declared elsewhere */
void load_users_from_file(); void load_events_from_file(); void load_bookings_from_file();
void load_notifications_from_file(); void load_waiting_from_file();
void save_users_to_file(); void save_events_to_file(); void save_bookings_to_file();
void save_notifications_to_file(); void save_waiting_to_file();

int register_user(); int login_user(char *out_username); int admin_login();
void add_event_interactive(); void view_events(); void edit_event_interactive(); void delete_event_interactive();
void view_feedback();

void book_ticket_flow(const char *username); void cancel_booking_flow(const char *username);
void view_bookings_for_user(const char *username); void view_payments_for_user(const char *username);
void add_notification(const char *username, const char *message); void view_notifications(const char *username);
void enqueue_waiting(const char *username, const char *phone, int eventId); 
WaitNode* dequeue_waiting_for_event(int eventId);

void load_all() {
    load_users_from_file();
    load_events_from_file();
    load_bookings_from_file();
    load_notifications_from_file();
    load_waiting_from_file();
}

void save_all() {
    save_users_to_file();
    save_events_to_file();
    save_bookings_to_file();
    save_notifications_to_file();
    save_waiting_to_file();
}

int main() {
    srand((unsigned)time(NULL));
    ensure_files_exist();
    load_all();

    extern void build_bst_from_bookings();
    build_bst_from_bookings();

    int ch;
    char username[50];

    while (1) {
        printf("\n=========================================\n");
        printf("     EVENT MANAGEMENT SYSTEM (DSA)\n");
        printf("=========================================\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("-----------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &ch) != 1) break;

        if (ch == 1) {
            register_user();
        } 
        else if (ch == 2) {
            if (login_user(username)) {
                int uc;
                while (1) {
                    printf("\n========== USER PANEL (%s) ==========\n", username);
                    printf("1. View Events\n");
                    printf("2. Book Ticket\n");
                    printf("3. Cancel Booking\n");
                    printf("4. My Bookings\n");
                    printf("5. Payments\n");
                    printf("6. Notifications\n");
                    printf("7. Feedback\n");
                    printf("8. Logout\n");
                    printf("=====================================\n");
                    printf("Enter your choice: ");

                    if (scanf("%d", &uc) != 1) {
                        printf("Invalid input. Please try again.\n");
                        while (getchar() != '\n');
                        continue;
                    }

                    switch (uc) {
                        case 1: view_events(); break;
                        case 2: book_ticket_flow(username); build_bst_from_bookings(); break;
                        case 3: cancel_booking_flow(username); build_bst_from_bookings(); break;
                        case 4: view_bookings_for_user(username); break;
                        case 5: view_payments_for_user(username); break;
                        case 6: view_notifications(username); break;
                        case 7: {
                            char msg[200];
                            printf("Enter feedback (no newline): ");
                            scanf(" %[^\n]", msg);
                            add_feedback(username, msg);
                            break;
                        }
                        case 8:
                            printf("Logging out from User Panel...\n");
                            goto exit_user;
                        default:
                            printf("Invalid choice! Please select 1–8.\n");
                    }
                }
                exit_user: ;
            } else {
                printf("Login failed.\n");
            }
        } 
        else if (ch == 3) {
            if (admin_login()) {
                int ac;
                while (1) {
                    printf("\n========== ADMIN PANEL ==========\n");
                    printf("1. Add Event\n");
                    printf("2. Edit Event\n");
                    printf("3. Delete Event\n");
                    printf("4. View Events\n");
                    printf("5. View Feedback\n");
                    printf("6. Logout\n");
                    printf("=================================\n");
                    printf("Enter your choice: ");

                    if (scanf("%d", &ac) != 1) {
                        printf("Invalid input. Please enter a number.\n");
                        while (getchar() != '\n');
                        continue;
                    }

                    switch (ac) {
                        case 1: add_event_interactive(); break;
                        case 2: edit_event_interactive(); break;
                        case 3: delete_event_interactive(); break;
                        case 4: view_events(); break;
                        case 5: view_feedback(); break;
                        case 6:
                            printf("Logging out from Admin Panel...\n");
                            goto exit_admin;
                        default:
                            printf("Invalid choice! Please select 1–6.\n");
                    }
                }
                exit_admin: ;
            } else {
                printf("Admin authentication failed.\n");
            }
        } 
        else {
            break;
        }
    }

    save_all();
    extern void bst_free(BSTNode*);
    bst_free(NULL);  // safe no-op
    printf("\nSystem Exiting... All data saved successfully.\n");
    return 0;
}
