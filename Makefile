CC=gcc
CFLAGS=-Wall -g -O3 -std=gnu99

all: test

test:
	$(CC) $(CFLAGS) -DTesting -Ilib src/uri_encode.c -o run-tests

clean:
	rm run-tests
