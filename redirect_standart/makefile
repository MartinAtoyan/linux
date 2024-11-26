all: redirect-cin

redirect-cin: redirect-cin.o
	g++ redirect-cin.o -o redirect-cin

redirect-cin.o: redirect-cin.cpp
	g++ -c -Wall redirect-cin.cpp

clean:
	rm -rf *.o redirect-cin

