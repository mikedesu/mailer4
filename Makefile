CC=gcc
CFLAGS=-g -Wall -Werror 

all: mail-in mail-out

mail-in: 
	$(CC) $(CFLAGS) main.c -o mail-in
mail-out: 
	$(CC) $(CFLAGS) mail-out.c -o mail-out
	sudo chown root:root mail-out
	sudo chmod 770 mail-out

clean:
	rm -rf mail-in mail-out tmpmail*

