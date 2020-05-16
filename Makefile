CC=gcc
CFLAGS=-g -Wall -Werror
INCLUDE=-Iinc
SERVER_SRC=server.c worker.c
COMMON_SRC=

all: server

server: $(COMMON_SRC) $(SERVER_SRC)
	$(CC) $(CFLAGS) $(INCLUDE) $(FLAGS) $^ -o $@ -pthread

clean:
	-rm server
	
.PHONY: clean