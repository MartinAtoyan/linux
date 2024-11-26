all: manual-append

manual-append: manual-append.o
	g++ manual-append.o -o manual-append

manual-append.o: manual-append.cpp
	g++ -c -Wall manual-append.cpp

clean:
	rm -rf *.o manual-append

