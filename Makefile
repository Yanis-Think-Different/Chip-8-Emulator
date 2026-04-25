COBJET = main.o display.o chip8.o keypad.o
CC = gcc
CFLAGS = -Wall -pedantic -std=c17 -g
CBIBLIO = -lSDL2

emulateur8 : $(COBJET)
	$(CC) $(CFLAGS) $(COBJET) -o emulateur8 $(CBIBLIO)

test : test.o chip8.o
	$(CC) $(CFLAGS) test.o chip8.o -o test

display.o : display.c display.h chip8.h
	$(CC) $(CFLAGS) -c display.c -o display.o

chip8.o : chip8.c chip8.h
	$(CC) $(CFLAGS) -c chip8.c -o chip8.o

keypad.o : keypad.c keypad.h chip8.h
	$(CC) $(CFLAGS) -c keypad.c -o keypad.o

main.o : main.c keypad.h chip8.h display.h
	$(CC) $(CFLAGS) -c main.c -o main.o

test.o : test.c chip8.h
	$(CC) $(CFLAGS) -c test.c -o test.o

clean :
	rm -f $(COBJET) emulateur8
