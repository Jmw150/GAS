

main:
	g++ src/main.cpp -o main

test:
	g++ tests/test.cpp -o test
	./test

clean:
	rm -f main test
