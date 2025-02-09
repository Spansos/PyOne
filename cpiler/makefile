CC=gcc
# CFLAGS := -g -Wall -Wextra -Wpedantic -Werror -Og
CFLAGS := -g -Wall -Wextra -Wpedantic -Og

SRCFILES = $(wildcard *.c)
OBJFILES = $(patsubst %.c,%.o,$(SRCFILES))

.PHONY: clean run

all: main

clean:
	rm *.o main

run: main
	./main

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main: $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@