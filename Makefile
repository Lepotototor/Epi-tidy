CC = gcc

CFLAGS =  -Wall -Wextra -Wvla -Werror
CPPFLAGS = -Iinclude
LDFLAGS = 

SRC = src/*.c
OBJ = check_files.o io_helper.o str_list.o epi-tidy.o

BIN = epi-tidy

all: $(BIN)

%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

debug: CFLAGS += -g -O0 -fsanitize=address
debug: LDFLAGS += -g -O0 -fsanitize=address
debug: $(BIN)

gdb: CFLAGS += -g -O0
gdb: LDFLAGS += -g -O0
gdb: $(BIN)

clean:
	$(RM) $(OBJ) $(BIN)

.PHONY: all debug clean gdb
