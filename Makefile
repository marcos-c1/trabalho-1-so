CFLAGS = -g -pthread
CC = gcc
run: sensores

sensores:
	$(CC) $(CFLAGS) -c sensores.c -o sensores.o 
	$(CC) $(CFLAGS) main.c -o main sensores.o

test:
	gcc test.c -o test

rm:
	rm *.o
