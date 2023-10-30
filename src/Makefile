CC=g++
CFLAGS=-Wall -Wextra -Wpedantic -Weffc++ -std=c++17

.PHONY: test clean

test: test/test_apy_int
	test/test_apy_int

test/test_apy_int: util.h apy_int.h apy_int.cc test/test_apy_int.cc test/catch.o
	${CC} ${CFLAGS} apy_int.cc test/test_apy_int.cc test/catch.o -o $@

test/catch.o: test/catch.cc test/catch.hpp
	${CC} ${CFLAGS} -c test/catch.cc -o test/catch.o

clean:
	@rm -rvf test/*.o
	@rm -rvf test/test_apy_int

