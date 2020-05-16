CC=gcc
CFLAGS=-g -Wall -Werror
SRC=main.c

all: sip

sip: $(SRC)
	$(CC) $(CFLAGS) $(FLAGS) $^ -o $@ 
	