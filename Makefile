CC=gcc
CFLAGS=-g -Wall -Werror
SERVER_SRC=server.c
CLIENT_SRC=client.c
COMMON_SRC=

all: server client

client: $(COMMON_SRC) $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(FLAGS) $^ -o $@ 

server: $(COMMON_SRC) $(SERVER_SRC)
	$(CC) $(CFLAGS) $(FLAGS) $^ -o $@ 
	