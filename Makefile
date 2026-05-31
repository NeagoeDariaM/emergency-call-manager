# NEAGOE Daria-Maria - 314CA
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=search_index

SRCS = main.c tasks.c queue.c list.c

.PHONY: build run clean

build: $(TARGETS)

search_index: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o search_index

run:
	./$(TARGETS)

clean:
	rm -f $(TARGETS)