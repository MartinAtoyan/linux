all: copy

copy: copy.o
	g++ copy.o -o copy

copy.o: copy.cpp
	g++ -c -Wall copy.cpp

clean:
	rm -rf *.o copy

