#
# vim: ts=8:colorcolumn=0
# Makefile for running tests, profiling, and coverage generation for the APyTypes C++ component
#

CC = g++
PYTHON3_INCLUDES = $(shell python3-config --includes)
PYTHON3_LINK = $(shell python3-config --ldflags --embed)
PYBIND11_INCLUDES = $(shell pybind11-config --includes)
WARNING_FLAGS = -Wall -Wextra -Wpedantic -Wno-deprecated -Weffc++
CXXFLAGS = ${WARNING_FLAGS} ${PYTHON3_INCLUDES} ${PYBIND11_INCLUDES} -std=c++17 -O2 -march=native -g

.PHONY: test test_profile test_coverage clean clean_all

TEST_SRC =				\
	test/test_apyfixed.cc		\
	test/test_arithmetic.cc		\
	test/test_string.cc

TEST_FLAGS=-D _IS_APYTYPES_UNIT_TEST --coverage

test: test/test_apytypes_util test/test_apyfixed
	test/test_apytypes_util
	test/test_apyfixed

test_profile: test/test_apytypes_util test/test_apyfixed
	valgrind --tool=callgrind test/test_apytypes_util
	valgrind --tool=callgrind test/test_apyfixed

test_coverage: test
	lcov --directory test --capture --output-file coverage.info
	genhtml --demangle-cpp -o coverage coverage.info

test/test_apytypes_util: ieee754.h apytypes_util.h test/test_apytypes_util.cc test/catch.o
	${CC} ${CXXFLAGS} ${TEST_FLAGS} test/test_apytypes_util.cc test/catch.o -o $@ -lgmp ${PYTHON3_LINK}

test/test_apyfixed: ieee754.h apytypes_util.h apyfixed.h apyfixed.cc ${TEST_SRC} test/catch.o
	${CC} ${CXXFLAGS} ${TEST_FLAGS} apyfixed.cc ${TEST_SRC} test/catch.o -o $@ -lgmp ${PYTHON3_LINK}

test/catch.o: test/catch.cc test/catch.hpp
	${CC} ${CXXFLAGS} -c test/catch.cc -o test/catch.o

#
# Regular soft cleaning. Does not remove test/catch.o, as that object file takes such a
# long time to build.
#
clean:
	@rm -rvf test/*.gcda
	@rm -rvf test/*.gcno
	@rm -rvf *.gcda
	@rm -rvf *.gcno
	@rm -rvf test/*.gcov
	@rm -rvf test/test_apyfixed
	@rm -rvf test/test_apytypes_util
	@rm -rvf callgrind.out.*
	@rm -rvf coverage/
	@rm -rvf coverage.info

#
# Hard cleaning. Remove everything build related
#
clean_all: clean
	@rm -rvf test/catch.o
