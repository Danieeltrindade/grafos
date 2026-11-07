CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=gnu11
LDLIBS = -lm
TARGET = main.exe
SRC = $(wildcard *.c)

all: $(TARGET)

$(TARGET): $(SRC) hamiltoniano.h tps.h
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

test: run

clean:
	rm -f main main.exe *.o

.PHONY: all run test clean