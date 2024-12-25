TARGET = find-max

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET)

