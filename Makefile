SERVER_APPLICATION = server.o
CLIENT_APPLICATION = client.o
SERVER_SRC_FILES = server.c functions/*.c
CLIENT_SRC_FILES = client.c
HEADER_FILE = threadbank.h
CC = cc
CFLAGS = -O2 -g -Wall -pedantic -pthread -D_GNU_SOURCE
LOGFILE = log_file.txt

all: $(SERVER_APPLICATION) $(CLIENT_APPLICATION)

$(SERVER_APPLICATION): $(SERVER_SRC_FILES) $(HEADER_FILE)
	$(CC) $(CFLAGS) -o $(SERVER_APPLICATION) $(SERVER_SRC_FILES)

$(CLIENT_APPLICATION): $(CLIENT_SRC_FILES)
	$(CC) $(CFLAGS) -o $(CLIENT_APPLICATION) $(CLIENT_SRC_FILES)

clean:
	rm -f *.o $(LOGFILE) server client *.DSYM

server-valgrind: $(SERVER_APPLICATION)
	valgrind --leak-check=full --show-leak-kinds=all ./$(SERVER_APPLICATION)

client-valgrind: $(SERVER_APPLICATION)
	valgrind --leak-check=full --show-leak-kinds=all ./$(CLIENT_APPLICATION)

server: $(SERVER_APPLICATION)
	./$(SERVER_APPLICATION)

client: $(CLIENT_APPLICATION)
	./$(CLIENT_APPLICATION)

test:
	./testbench/as2_testbench ${CLIENT_APPLICATION}

