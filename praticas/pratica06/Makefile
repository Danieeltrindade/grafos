
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic
TARGET = main
SRC = main.c dijkstra.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c dijkstra.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJ)

run: $(TARGET)
	./$(TARGET)
