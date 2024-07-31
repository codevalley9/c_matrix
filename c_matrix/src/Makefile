CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
LIBS = -lcheck
GCOV = -fprofile-arcs -ftest-coverage
SRCS = s21_matrix.c
OBJS = $(SRCS:.c=.o)
OS := $(shell uname -s)

all: s21_matrix

s21_matrix: s21_matrix.a
	$(CC) TEST/test.c -lcheck -L. s21_matrix.a -o s21_matrix

s21_matrix.a:
	$(CC) -c $(FLAGS) $(SRCS)
	ar rcs s21_matrix.a $(OBJS)
	ranlib s21_matrix.a

test: s21_matrix.a
	$(CC) TEST/test.c -lcheck -L. s21_matrix.a -o s21_test_matrix
	./s21_test_matrix

gcov_report:
	$(CC) $(GCOV) TEST/test.c $(SRCS) -o s21_test_matrix -lcheck
	./s21_test_matrix
	lcov -t "test" -o test.info -c -d ./
	genhtml test.info -o report
	open report/index.html

clean:
	rm -rf *.a *.o *.info *.gcno *.gcda *.gcov 
	rm -rf s21_test_matrix report a.out s21_matrix tests_matrix.c .clang-format a.out.dSYM

valgrind_check: test
	CK_FORK=no valgrind --tool=memcheck ./s21_test_matrix


leaks: clean test
	CK_FORK=no leaks -atExit -- ./s21_test_matrix


style:
	cp ../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	clang-format -n TEST/test.c *.c *.h