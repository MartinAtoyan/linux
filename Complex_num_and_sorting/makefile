all: complex_demo

complex_demo: main.o ComplexNumber.o ComplexSort.o
	g++ main.o ComplexNumber.o ComplexSort.o -o complex_demo

main.o: main.cpp
	g++ -c -Wall main.cpp

ComplexNumber.o: ComplexNumber.cpp
	g++ -c -Wall ComplexNumber.cpp

ComplexSort.o: ComplexSort.cpp
	g++ -c -Wall ComplexSort.cpp

clean:
	rm -rf *.o complex_demo

