CC = g++
LFLAGS = -Wall -std=c++14 -g -O0 -pedantic-errors 
shell:shell.cpp
	$(CC) $(LFLAGS) -o shell shell.cpp
