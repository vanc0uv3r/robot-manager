CC=g++

CFLAGS= -Wall -g

all:
	g++ -Wall main.cpp helpers.cpp Robot.cpp Competitor.cpp -o robot
	g++ -Wall lexer.cpp Lex.cpp -o lexer
