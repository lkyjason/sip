CC=gcc
CFLAGS=-g -Wall -Werror
SERVER_SRC=server.c
COMMON_SRC=

all: server

server: $(COMMON_SRC) $(SERVER_SRC)
	$(CC) $(CFLAGS) $(FLAGS) $^ -o $@ 

clean:
	-rm server
	
.PHONY: clean