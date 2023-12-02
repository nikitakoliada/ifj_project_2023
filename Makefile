##
 # Project: Implementation of imperative language compiler IFJ23
 # @file Makefile
 #
 # @brief Makefile implementation
 #
 # @author Juraj Remeň - xremen02
##

CC = gcc
FLAGS = -std=c99
OUT = IFJ23
SRCS := $(wildcard *.c)
GENERATOR := generator.c generator.h

all:
	${CC} ${FLAGS} ${SRCS} -o ifj

.PHONY: generator_test
generator_test:
	rm -rf tests/generator_test.out tests/generator_test.ifjc23
	${CC} ${FLAGS} ${GENERATOR} -o generator_test
	./generator_test > tests/generator_test.ifjc23
	./tests/arch.sh

clean:
	rm -rf ifj generator_test