CXX = g++
CXXFLAGS = -Wall -std=c++11


TARGET = read-file


all: $(TARGET)


$(TARGET): read-file.o
	$(CXX) $(CXXFLAGS) read-file.o -o $(TARGET)


read-file.o: read-file.cpp
	$(CXX) -c $(CXXFLAGS) read-file.cpp


clean:
	rm -rf *.o $(TARGET)

