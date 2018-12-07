CXX=g++
CXXFLAGS=-std=c++11 -O3

all: example.out
example.out: example.cpp updatable_priority_queue.h
	$(CXX) $(CXXFLAGS) -o example.out example.cpp
example: example.out
	./example.out
clean:
	rm -f example.out
