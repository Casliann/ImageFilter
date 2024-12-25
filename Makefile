CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter -fsanitize=address

default: imagefilter
imagefilter: ./src/main.c ./src/utils.c ./src/filters.c 
	$(CC) $(CFLAGS) ./src/main.c ./src/utils.c ./src/filters.c -o imagefilter
clear:
	rm imagefilter
