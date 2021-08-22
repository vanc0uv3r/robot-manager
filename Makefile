CC=g++

CFLAGS= -Wall -g

all: bot

bot: main.o robot.o helpers.o competitor.o lexer.o
	g++ main.o robot.o helpers.o competitor.o lexer.o -o robot_manager
	rm -rf *.o

main.o: syntaxer.cpp
	g++ -c main.cpp -o main.o

helpers.o: helpers/helpers.cpp
	g++ -c helpers/helpers.cpp -o helpers.o

robot.o: Robot/Robot.cpp Robot/Robot.h
	g++ -c Robot/Robot.cpp -o robot.o

competitor.o: Competitor/Competitor.cpp
	g++ -c Competitor/Competitor.cpp -o competitor.o

lexer.o: Lex/Lex.cpp Lex/Lex.h
	g++ -c Lex/Lex.cpp -o lexer.o