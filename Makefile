SERVER_APPLICATION = server.o
CLIENT_APPLICATION = client.o
SERVER_SRC_FILES = server.c functions/*.c bank_actions/*.c
CLIENT_SRC_FILE = client.c
HEADER_FILE = threadbank.h
CC = cc
CFLAGS = -O2 -g -Wall -pedantic -pthread
CLEAN_TARGETS = log_file.txt accounts/*

all: $(SERVER_APPLICATION) $(CLIENT_APPLICATION)

$(SERVER_APPLICATION): $(SERVER_SRC_FILES) $(HEADER_FILE)
	$(CC) $(CFLAGS) -o $(SERVER_APPLICATION) $(SERVER_SRC_FILES)

$(CLIENT_APPLICATION): $(CLIENT_SRC_FILE)
	$(CC) $(CFLAGS) -o $(CLIENT_APPLICATION) $(CLIENT_SRC_FILE)

clean:
	rm -f *.o $(CLEAN_TARGETS) $(SERVER_APPLICATION) $(CLIENT_APPLICATION)

server: $(SERVER_APPLICATION)
	./$(SERVER_APPLICATION)

client: $(CLIENT_APPLICATION)
	./$(CLIENT_APPLICATION)

re: clean all

server-valgrind: $(SERVER_APPLICATION)
	valgrind --leak-check=full --show-leak-kinds=all ./$(SERVER_APPLICATION)

client-valgrind: $(SERVER_APPLICATION)
	valgrind --leak-check=full --show-leak-kinds=all ./$(CLIENT_APPLICATION)

	