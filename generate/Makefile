generate: generate.o socket.o
	gcc -g -Wall generate.o socket.o -o generate -pthread -lm

generate.o: generate.c
	gcc -c -Wall generate.c -o generate.o

socket.o: socket.c
	gcc -c -Wall socket.c -o socket.o

.PHONY:
clean:
	rm -rf *~
	rm -rf *.o
	rm -rf a.out
	rm -rf generate

.PHONY:
all: clean generate
