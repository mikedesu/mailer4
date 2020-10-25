#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MAIL_FROM_PREFIX_SIZE 10

void handle_mailfrom(char *line);

int main(int argc, char **argv) {
    char line[BUFFER_SIZE] = {0};
    // read first line
    char *fgets_result = fgets(line, BUFFER_SIZE, stdin);
    // continue reading lines until we have no more
    while (fgets_result != NULL) {
        // just print out the line
        printf("%s", line);
        handle_mailfrom(line);   
        fgets_result = fgets(line, BUFFER_SIZE, stdin);
    }
    return 0;
}

void handle_mailfrom(char *line) {
    // check to see if the first 10 chars are "MAIL FROM:"
    int r = strncmp(line, "MAIL FROM:", MAIL_FROM_PREFIX_SIZE);
    if (r==0) {

        // read in "MAIL FROM", handle appropriately
        char from_user[1024] = {0};
        // copy just the part of the line after "MAIL FROM:"
        strcpy(from_user, line + MAIL_FROM_PREFIX_SIZE );
        // print the user out just as proof we've successfully parsed the line
        printf("From user: %s\n", from_user);
    }
    else {
        printf("Error, expecting MAIL FROM, exiting...\n");
        exit(-1);
    }
}
