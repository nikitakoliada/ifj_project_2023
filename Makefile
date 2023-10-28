##
 # Project: Implementation of imperative language compiler IFJ23
 # @file Makefile
 #
 # @brief Makefile implementation
 #
 # @author Juraj Remeň - xremen02
##

CC = gcc
FLAGS = -std=c99 -pedantic -Wall -Wextra
OUT = IFJ23
SRCS := $(wildcard *.c)

all:
	${CC} ${FLAGS} ${SRCS} -o ifj

clean:
	rm -rf ifj