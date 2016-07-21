CFLAGS=-c -Wall -O -std=c99

all: main.o ghc.o
	gcc -std=c99 -o bin/ghc_test bin/main.o bin/ghc.o

ghc.o: src/ghc.c src/ghc.h
	gcc $(CFLAGS) src/ghc.c -o bin/ghc.o

main.o: src/main.c src/ghc.h
	gcc $(CFLAGS) src/main.c -o bin/main.o

clean:
	rm -f bin/*

check: all
	bin/ghc_test
