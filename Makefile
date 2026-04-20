
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./src

.PHONY: main test clean

main:
	$(CXX) $(CXXFLAGS) src/main.cpp -o main

test:
	$(CXX) $(CXXFLAGS) tests/test.cpp -o test
	./test
	$(CXX) $(CXXFLAGS) tests/test_word.cpp -o test_word
	./test_word
	$(CXX) $(CXXFLAGS) tests/test_factorial.cpp -o test_factorial
	./test_factorial

clean:
	rm -f main test test_word test_factorial
