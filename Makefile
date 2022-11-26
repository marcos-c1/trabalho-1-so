run: sensores main

sensores:
	gcc -c sensores.c -o sensores.o
	
main:
	gcc sensores.o main.c -o main.o

test:
	gcc test.c && ./a.out

rm:
	rm *.o
