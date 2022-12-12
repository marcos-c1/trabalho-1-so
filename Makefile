CFLAGS = -Wall -g -pthread
TESTFLAGS = -g -pthread
CC = gcc
OBJDIR = ./obj
SRCDIR = ./src
INCLUDE_SSH = -L/usr/local/lib/ -lssl -lcrypto

run: sensores

sensores:
	$(CC) $(CFLAGS) -c $(SRCDIR)/sensores.c -o $(OBJDIR)/sensores.o 
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $(OBJDIR)/main.o $(OBJDIR)/sensores.o

test:
	$(CC) $(TESTFLAGS) $(SRCDIR)/test.c $(INCLUDE_SSH) -o $(OBJDIR)/test.o

rm:
	rm $(OBJDIR)/*.o