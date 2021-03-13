CC=g++

CFLAGS= -Wall -g

all:
	g++ main.cpp helpers.cpp Robot.cpp -o robot
