CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/main.c \
      src/cli.c \
      src/trace.c \
      src/fifo.c \
      src/lru.c \
      src/clock.c \
      src/memory.c \
      src/metrics.c \
      src/thrashing.c

OBJ = $(SRC:.c=.o)
TARGET = simulator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)