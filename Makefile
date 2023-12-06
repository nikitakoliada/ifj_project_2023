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

all: ifj

ifj: scanner.o symtable.o stack.o analysis.o expression.o generator.o main.c
	${CC} ${FLAGS} -o $@ $^
	rm -rf *.o *h.gch

.PHONY: ifj

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf ifj *.o