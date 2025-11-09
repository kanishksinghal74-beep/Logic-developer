#include "structures.h"

void process_payment_record(const char *username, const char *bookingId, int amount, char *out_paymentId) {
    int mode; printf("\n--- Payment ---\n1.UPI 2.Card 3.Cash\nChoose: "); scanf("%d", &mode);
    char mode_s[20];
    if (mode==1) strcpy(mode_s,"UPI"); else if (mode==2) strcpy(mode_s,"Card"); else strcpy(mode_s,"Cash");
    snprintf(out_paymentId,20,"PAY%d", rand()%900000 + 1000);
    FILE *fp = fopen("files/payments.txt","a");
    if (!fp) { printf("Payment file error\n"); return; }
    fprintf(fp,"%s %s %d %s %d %s\n", out_paymentId, username, 0, mode_s, amount, "SUCCESS");
    fclose(fp);
    printf("Payment saved: %s\n", out_paymentId);
}

void view_payments_for_user(const char *username) {
    FILE *fp = fopen("files/payments.txt","r");
    if (!fp) { printf("No payments.\n"); return; }
    char pid[20], user[50], mode[20], status[20]; int eid, amt; int found=0;
    while (fscanf(fp,"%19s %49s %d %19s %d %19s", pid, user, &eid, mode, &amt, status) == 6) {
        if (strcmp(user, username)==0) {
            printf("%s | %d | %s | %d | %s\n", pid, eid, mode, amt, status); found=1;
        }
    }
    if (!found) printf("No payments for %s\n", username);
    fclose(fp);
}

