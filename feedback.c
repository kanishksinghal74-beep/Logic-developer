#include "structures.h"

void add_feedback(const char *username, const char *message) {
    FILE *fp = fopen("files/feedback.txt","a");
    if (!fp) { printf("Feedback file error\n"); return; }
    fprintf(fp, "%s: %s\n", username, message);
    fclose(fp);
    printf("Feedback saved.\n");
}

void view_feedback() {
    FILE *fp = fopen("files/feedback.txt","r");
    if (!fp) { printf("No feedback.\n"); return; }
    char line[300];
    printf("\n--- Feedback ---\n");
    while (fgets(line, sizeof(line), fp)) printf("%s", line);
    fclose(fp);
}

