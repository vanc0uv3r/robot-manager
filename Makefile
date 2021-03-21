CC=g++

CFLAGS= -Wall -g

all:
	g++ main.cpp helpers.cpp Robot.cpp Competitor.cpp -o robot
