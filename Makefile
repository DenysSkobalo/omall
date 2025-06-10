.PHONY: compile run clean

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
FILE = main.c
EXEC = main

all: run

compile:
	$(CC) $(CFLAGS) -g -o $(EXEC) $(FILE)

run: compile
	./$(EXEC)

clean:
	rm $(EXEC)

