all: main.e

main.e: main.o output.o quadtree.o read.o
	gcc -o main.e main.o output.o quadtree.o read.o
	
main.o: main.c
	gcc -o main.o -c main.c
	
output.o: output.c
	gcc -o output.o -c output.c

quadtree.o: quadtree.c
	gcc -o quadtree.o -c quadtree.c

read.o: read.c
	gcc -o read.o -c read.c
	
clean:
	rm -f *.o main.e