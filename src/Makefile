CC=g++
CFLAGS=								\
	-Wall -Wextra -Wpedantic -Wno-depricated -Weffc++ 	\
	-D _GLIBCXX_DEBUG -std=c++17 -O3 -march=native -g

.PHONY: test test_profile test_coverage clean clean_all

TEST_SRC =				\
	test/test_apy_fixed.cc		\
	test/test_binary_operator.cc	\
	test/test_to_from_string.cc

TEST_FLAGS=-D _IS_APY_TYPES_UNIT_TEST --coverage

test: test/test_apy_util test/test_apy_fixed
	test/test_apy_util
	test/test_apy_fixed

test_profile: test/test_apy_util test/test_apy_fixed
	valgrind --tool=callgrind test/test_apy_util
	valgrind --tool=callgrind test/test_apy_fixed

test_coverage: test
	lcov --directory test --capture --output-file coverage.info
	genhtml --demangle-cpp -o coverage coverage.info

test/test_apy_util: apy_util.h test/test_apy_util.cc test/catch.o
	${CC} ${CFLAGS} ${TEST_FLAGS} test/test_apy_util.cc test/catch.o -o $@ -lgmp

test/test_apy_fixed: apy_util.h apy_fixed.h apy_fixed.cc ${TEST_SRC} test/catch.o
	${CC} ${CFLAGS} ${TEST_FLAGS} apy_fixed.cc ${TEST_SRC} test/catch.o -o $@ -lgmp

test/catch.o: test/catch.cc test/catch.hpp
	${CC} ${CFLAGS} -c test/catch.cc -o test/catch.o

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
	@rm -rvf test/test_apy_fixed
	@rm -rvf test/test_apy_util
	@rm -rvf callgrind.out.*
	@rm -rvf coverage/
	@rm -rvf coverage.info

#
# Hard cleaning. Remove everything build related
#
clean_all: clean
	@rm -rvf test/catch.o

