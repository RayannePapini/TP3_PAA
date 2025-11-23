CC = gcc

CFLAGS = -Wall -Wextra -Iinclude -g

TARGET = main.exe

SRCS = $(shell find src -name '*.c')

OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)