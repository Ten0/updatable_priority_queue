CXX=g++
CXXFLAGS=-std=c++11 -O3

all: example.out test.out
example.out: example.cpp updatable_priority_queue.h
	$(CXX) $(CXXFLAGS) -o example.out example.cpp
example: example.out
	./example.out
test.out: test.cpp updatable_priority_queue.h
	$(CXX) $(CXXFLAGS) -o test.out test.cpp
test: test.out
	./test.out
clean:
	rm -f example.out test.out
