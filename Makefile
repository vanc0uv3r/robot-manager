CC=g++

CFLAGS= -Wall -g

all: bot

bot: main.o robot.o competitor.o lexer.o syntaxer.o
	g++ main.o robot.o competitor.o lexer.o syntaxer.o -o robot

main.o: main.cpp helpers.cpp
	g++ main.cpp helpers.cpp

robot.o: Robot.cpp
	g++ Robot.cpp

competitor.o: Competitor.cpp
	g++ Competitor.cpp

lexer.o: lexer.cpp Lex.h Lex.cpp
	g++ lexer.cpp Lex.cpp

syntaxer.o: syntaxer.cpp Syntax.cpp Syntax.h
	g++ syntaxer.cpp Syntax.cpp -o syntaxer
