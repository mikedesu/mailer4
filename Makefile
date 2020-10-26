CC=gcc
CFLAGS=-g -Wall -Werror 

all: mail-in

mail-in: 
	$(CC) $(CFLAGS) main.c -o mail-in

clean:
	rm -rf mail-in 

