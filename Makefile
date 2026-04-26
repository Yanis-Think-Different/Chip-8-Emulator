COBJET = main.o display.o chip8.o keypad.o
CC = gcc
CFLAGS = -Wall -pedantic -std=c23 -g
CBIBLIO = -lSDL2

emulateur8 : $(COBJET)
	$(CC) $(CFLAGS) $(COBJET) -o emulateur8 $(CBIBLIO)

test : test.o chip8.o
	$(CC) $(CFLAGS) test.o chip8.o -o test

chip8.o : chip8.c chip8.h
	$(CC) $(CFLAGS) -c chip8.c -o chip8.o

main.o : main.c chip8.h
	$(CC) $(CFLAGS) -c main.c -o main.o

test.o : test.c chip8.h
	$(CC) $(CFLAGS) -c test.c -o test.o

clean :
	rm -f $(COBJET) emulateur8
