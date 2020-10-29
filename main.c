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



int num_valid_usernames = 0;
char **valid_usernames = NULL;
int receipt_to_handled = 0;


//int lines_in_file(FILE *infile);
//void read_valid_usernames();
void handle_mailfrom(char *line, char *from_user);
void handle_receipt_to(char *line, int *num_recipients, char *recipients[]);
//int is_valid_username(char *username);


int main(int argc, char *argv[]) {
    int mailfrom_read_in = 0;
    char from_user[1024] = {0};
    int num_recipients = 0;
    char *recipients[BUFFER_SIZE] = {0};
    char line[BUFFER_SIZE] = {0};
    //read_valid_usernames();
    // read first line
    char *fgets_result = fgets(line, BUFFER_SIZE, stdin);
    // continue reading lines until we have no more
    while (fgets_result != NULL) {
        // just print out the line
        if ( ! mailfrom_read_in ) {
            handle_mailfrom(line, from_user);   
            
            printf("From: %s\n", from_user);
            
            mailfrom_read_in = 1;
        }
        else if (receipt_to_handled == 0) {
            handle_receipt_to(line, &num_recipients, recipients);
        }
        else {
            // just print the line because that's what happens here
            // we will have to come back and scrub the lines for periods
            // as well as handle end-of-mail and possible other mails in this file
            printf("%s", line);
        }
        fgets_result = fgets(line, BUFFER_SIZE, stdin);
    }
    return 0;
}


void handle_mailfrom(char *line, char *from_user){
    //printf("handle_mailfrom: [%s] [%s]\n", line, from_user);
    // check to see if the first 10 chars are "MAIL FROM:"
    int r = strncmp(line, "MAIL FROM:", MAIL_FROM_PREFIX_SIZE);
    if (r==0) {
        // read in "MAIL FROM", handle appropriately
        // copy just the part of the line after "MAIL FROM:"
        strcpy(from_user, line + MAIL_FROM_PREFIX_SIZE );
        from_user[ strlen(from_user)-1 ] = 0;
        //if ( is_valid_username(from_user) == -1 ) {
        //    printf("Invalid user: %s, exiting\n", from_user);
        //    exit(-1);
        //}
        // print the user out just as proof we've successfully parsed the line
    }
    else {
        printf("Error, expecting MAIL FROM, exiting...\n");
        exit(-1);
        //handle_receipt_to(line);
    }
}


void handle_receipt_to(char *line, int *num_recipients, char *recipients[]) {
    //printf("handle_receipt_to: [%s] [%d]\n", line, *num_recipients);
    int r = strncmp(line, "RCPT TO:", RCPT_TO_PREFIX_SIZE);
    int r0 = strncmp(line, "DATA", 4);
    int r1 = strncmp(line, "\n", 1);
    if (r==0) {
        //printf("--is receipt line--\n");   
        int index = *num_recipients;
        recipients[index] = (char *)calloc(sizeof(char), BUFFER_SIZE);
        if (recipients[index]==NULL) {
            printf("Error callocing recipients[index], exiting...\n");
            exit(-1);
        }
        strcpy(recipients[index], line + RCPT_TO_PREFIX_SIZE); 
        recipients[index][ strlen( recipients[index] ) - 1 ] = 0;
        //if ( is_valid_username( recipients[index] ) == -1 ) {
        //    printf("Invalid user: %s, exiting\n", recipients[index]);
        //    exit(-1);
        //}
        printf("To: %s\n", recipients[index]);
        (*num_recipients)++;
    }
    else if (r0 == 0) {
        //printf("handle DATA\n\n");
        receipt_to_handled = 1;
    }
    else if (r1 == 0) {
        fprintf(stderr, "Error processing handle_receipt_to, line contained blank line\n" );
        exit(-1);
    }
    else {
        fprintf(stderr, "Error processing handle_receipt_to, line contained: [%s]\n", line);
        exit(-1);
    }
}


// Returns 0 on success, -1 on failure
/*
int is_valid_username(char *username) {
    int result = -1;
    for (int i = 0; i < num_valid_usernames; i++) {
        if ( strcmp(username, valid_usernames[i]) == 0 ) {
            result = 0;
            break;
        }
    }
    return result;
}
*/

/*
void read_valid_usernames() {
    char *filename = "valid_usernames";
    FILE *infile = fopen(filename, "r");
    if (infile==NULL) {
        perror("Error opening file, exiting");
        exit(-1);
    }
    
    int linecount = lines_in_file(infile) - 1;
    num_valid_usernames = linecount;
    //printf("Valid username count: %d\n", linecount);
    rewind(infile);

    // actually malloc username array and read in usernames
    valid_usernames = (char **)calloc(sizeof(char*), num_valid_usernames);
    if (valid_usernames==NULL) {
        perror("Error callocing valid_usernames, exiting");
        exit(-1);
    }

    for (int i = 0; i < num_valid_usernames; i++) {
        
        valid_usernames[i] = (char *)calloc(sizeof(char), BUFFER_SIZE);
        if (valid_usernames[i]==NULL) {
            perror("Error callocing valid_usernames[i], exiting");
            exit(-1);
        }
        fgets(valid_usernames[i], BUFFER_SIZE, infile);
        // chop off the newline by overwriting it with a NULL or '\0' or 0 byte
        int end_of_line = strlen(valid_usernames[i]);
        valid_usernames[i][end_of_line-1] = 0;
        //printf("Proof: %s\n", valid_usernames[i]);
    }
    fclose(infile);
}
*/

/*
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
*/

