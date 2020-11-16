CC=gcc
CFLAGS=-g -Wall -Werror 
TREE?=mytree

all: mail-in mail-out

mail-in: 
	$(CC) $(CFLAGS) main.c -o mail-in

mail-out: 
	$(CC) $(CFLAGS) mail-out.c -o mail-out

install:
	make clean
	make
	./install-unpriv $(TREE) 
	sudo ./install-priv $(TREE)

clean:
	rm -rf mail-in mail-out tmpmail*

