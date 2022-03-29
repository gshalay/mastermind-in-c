# makefile.mk
# Name: Greg Shalay
# File Description: Contains the commands to compile code for mastermind game.

CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: master.o master_main

clean:
	rm *.o
	rm master_main

# EXECUTABLE FILE COMPILATION COMMANDS
master_main: master_main.o
	$(CC) $(CFLAGS) -o master_main master.o master_main.o

# OBJECT FILE COMPILATION COMMANDS
master.o: master.h master.c
	$(CC) $(CFLAGS) master.c -o master.o -c
	
master_main.o: master.h master_main.c
	$(CC) $(CFLAGS) master_main.c -o master_main.o -c