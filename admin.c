#include "structures.h"

int admin_login() {
    char u[50], p[50];
    printf("Admin Username: "); scanf("%49s", u);
    printf("Admin Password: "); scanf("%49s", p);
    if (strcmp(u,"admin")==0 && strcmp(p,"123")==0) return 1;
    return 0;
}

