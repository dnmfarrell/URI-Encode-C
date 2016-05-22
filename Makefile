CC=gcc
CFLAGS=-Wall -g -std=gnu99

all: test

test:
	$(CC) $(CFLAGS) -DTesting -Ilib src/uri_encode.c -o uri_encode

clean:
	rm uri_encode
