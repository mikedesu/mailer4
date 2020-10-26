#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE           1024
#define MAIL_FROM_PREFIX_SIZE 10
#define RCPT_TO_PREFIX_SIZE   8

/*
1. checking if a username is a valid username
2. DATA handling
3. End of DATA (.) 
*/

int lines_in_file(FILE *infile);
void read_valid_usernames();
void handle_mailfrom(char *line, char *from_user);
void handle_receipt_to(char *line, int *num_recipients, char *recipients[]) ;

int main(int argc, char *argv[]) {
    int mailfrom_read_in = 0;
    char from_user[1024] = {0};
    int num_recipients = 0;
    char *recipients[BUFFER_SIZE] = {0};
    char line[BUFFER_SIZE] = {0};


    read_valid_usernames();

    // read first line
    char *fgets_result = fgets(line, BUFFER_SIZE, stdin);
    // continue reading lines until we have no more
    while (fgets_result != NULL) {
        // just print out the line
        printf("%s", line);
        if ( ! mailfrom_read_in ) {
            handle_mailfrom(line, from_user);   
            printf("%s\n", from_user);
            mailfrom_read_in = 1;
        }
        else {
            handle_receipt_to(line, &num_recipients, recipients);
        }
        fgets_result = fgets(line, BUFFER_SIZE, stdin);
    }
    printf("Num recipients: %d\n", num_recipients);
    for (int i =0 ; i < num_recipients; i++) {
        printf("%s\n", recipients[i]);
    }
    return 0;
}

void handle_mailfrom(char *line, char *from_user){
    // check to see if the first 10 chars are "MAIL FROM:"
    int r = strncmp(line, "MAIL FROM:", MAIL_FROM_PREFIX_SIZE);
    if (r==0) {
        // read in "MAIL FROM", handle appropriately
        // copy just the part of the line after "MAIL FROM:"
        strcpy(from_user, line + MAIL_FROM_PREFIX_SIZE );
        // print the user out just as proof we've successfully parsed the line
    }
    else {
        printf("Error, expecting MAIL FROM, exiting...\n");
        exit(-1);
        //handle_receipt_to(line);
    }
}

void handle_receipt_to(char *line, int *num_recipients, char *recipients[]) {
    int r = strncmp(line, "RCPT TO:", RCPT_TO_PREFIX_SIZE);
    if (r==0) {
        //printf("--is receipt line--\n");   
        int index = *num_recipients;
        
        recipients[index] = (char *)calloc(sizeof(char), BUFFER_SIZE);
        if (recipients[index]==NULL) {
            printf("Error callocing recipients[index], exiting...\n");
            exit(-1);
        }

        strcpy(recipients[index], line + RCPT_TO_PREFIX_SIZE); 

        (*num_recipients)++;
    }
    else {
        printf("Error, expecting RCPT TO, exiting...\n");
        exit(-1);
    }
}


void read_valid_usernames() {
    char *filename = "valid_usernames";
    FILE *infile = fopen(filename, "r");
    if (infile==NULL) {
        perror("Error opening file, exiting");
        exit(-1);
    }
    int lc = lines_in_file(infile);
    printf("Valid username count: %d\n", lc);
    rewind(infile);

    // actually malloc username array and read in usernames

    fclose(infile);
}


int lines_in_file(FILE *infile) {
    // count lines in file
    int lines = 0;
    char tmp[1024]={0};
    while (!feof(infile)) {
        fgets(tmp, 1024, infile);   
        lines++;
    }
    return lines;
}


