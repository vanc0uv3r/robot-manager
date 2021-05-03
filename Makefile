CC=g++

CFLAGS= -Wall -g

all: bot

bot: main.o robot.o helpers.o competitor.o syntaxer.o lexer.o
	g++ main.o robot.o helpers.o competitor.o syntaxer.o lexer.o -o kek
	rm -rf *.o

main.o: syntaxer.cpp
	g++ -c syntaxer.cpp -o main.o

helpers.o: helpers/helpers.cpp
	g++ -c helpers/helpers.cpp -o helpers.o

robot.o: Robot.cpp
	g++ -c Robot.cpp -o robot.o

competitor.o: Competitor/Competitor.cpp
	g++ -c Competitor/Competitor.cpp -o competitor.o

syntaxer.o: Syntax/Syntax.cpp Syntax/Syntax.h
	g++ -c Syntax/Syntax.cpp -o syntaxer.o

lexer.o: Lex/Lex.cpp Lex/Lex.h
	g++ -c Lex/Lex.cpp -o lexer.o
