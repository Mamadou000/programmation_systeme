# Makefile
CC = gcc
CFLAGS = -g -Wall  
LDFLAGS =
OBJ = main.o mini_memory.o mini_string.o mini_io.o mini_commandes.o mini_shell.o
run: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o run
main.o: main.c mini_lib.h
mini_memory.o : mini_memory.c mini_lib.h
mini_string.o: mini_string.c mini_lib.h
mini_io.o: mini_io.c mini_lib.h
mini_commandes: mini_commandes.c mini_lib.h
mini_shell.o: mini_shell.c mini_lib.h
clean :
	rm -f $(OBJ) run