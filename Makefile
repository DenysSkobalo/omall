CC = gcc
FLAGS = -o
FILE = main.c
EXEC = main
compile:
	$(CC) $(FLAGS) $(EXEC) $(FILE)

run: 
	./$(EXEC)

clean:
	rm $(EXEC)

.PHONY: compile run clean