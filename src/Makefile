CC=g++
CFLAGS=-Wall -Wextra -Wpedantic -Weffc++ -std=c++17 -O2

.PHONY: test clean

test: test/test_apy_util test/test_apy_fixed
	test/test_apy_util
	test/test_apy_fixed

test/test_apy_util: apy_util.h test/test_apy_util.cc test/catch.o
	${CC} ${CFLAGS} test/test_apy_util.cc test/catch.o -o $@

test/test_apy_fixed: apy_util.h apy_fixed.h apy_fixed.cc test/test_apy_fixed.cc test/catch.o
	${CC} ${CFLAGS} apy_fixed.cc test/test_apy_fixed.cc test/catch.o -o $@


test/catch.o: test/catch.cc test/catch.hpp
	${CC} ${CFLAGS} -c test/catch.cc -o test/catch.o

clean:
	@rm -rvf test/*.o
	@rm -rvf test/test_apy_fixed

