all: hacker-rm

hacker-rm: hacker-rm.o
	g++ hacker-rm.o -o hacker-rm

hacker-rm.o: hacker-rm.cpp
	g++ -c -Wall hacker-rm.cpp

clean:
	rm -rf *.o hacker-rm

