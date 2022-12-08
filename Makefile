CFLAGS = -Wall -g -pthread
CC = gcc
OBJDIR = ./obj
SRCDIR = ./src

run: sensores

sensores:
	$(CC) $(CFLAGS) -c $(SRCDIR)/sensores.c -o $(OBJDIR)/sensores.o 
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(OBJDIR)/sensores.o

test:
	$(CC) $(CFLAGS) $(SRCDIR)/test.c -o $(OBJDIR)/test.o

rm:
	rm $(OBJDIR)/*.o
