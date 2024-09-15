all: main.e

main.e: main.o output.o rebuild.o
	gcc -o main.e main.o output.o rebuild.o
	
main.o: main.c
	gcc -o main.o -c main.c
	
output.o: output.c
	gcc -o output.o -c output.c

rebuild.o: rebuild.c
	gcc -o rebuild.o -c rebuild.c
	
clean:
	rm -f *.o main.e