##
 # Project: Implementation of imperative language compiler IFJ23
 # @file Makefile
 #
 # @brief Makefile implementation
 #
 # @author Nikita Koliada - xkolia00
##

CC = gcc
FLAGS = -std=c99 -pedantic -Wall -Wextra
OUT = IFJ23
GENERATOR := generator.c generator.h

all: ifj

ifj: scanner.o symtable.o stack.o analysis.o expression.o generator.o main.c
	${CC} ${FLAGS} -o $@ $^

.PHONY: generator_test ifj
generator_test:
	rm -rf tests/generator_test.out tests/generator_test.ifjc23
	${CC} ${FLAGS} ${GENERATOR} -o generator_test
	./generator_test > tests/generator_test.ifjc23
	./tests/arch.sh

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf ifj *.o generator_test