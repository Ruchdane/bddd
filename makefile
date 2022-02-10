CC = gcc
SRC = src
HEADER = header
BIN = bin
INCLUDE = -I $(HEADER)
CFLAGS = -g  $(INCLUDE)
LINKER = -Lbin -lsda -Wl,-rpath=./bin


all : server client

$(BIN)/%.o : $(SRC)/%.c
	$(CC)  $(CFLAGS) $^ -c -o $@

server:  $(BIN)/db.o $(BIN)/protocole.o $(BIN)/network.o $(BIN)/server.o $(BIN)/log.o
	 $(CC)  $(CFLAGS) $^ -o dist/$@  -lpthread $(LINKER)
client: $(BIN)/shell.o $(BIN)/protocole.o $(BIN)/network.o $(BIN)/client.o
	 $(CC)  $(CFLAGS) $^ -o dist/$@ $(LINKER)

clean:
	rm -r bin/*.o