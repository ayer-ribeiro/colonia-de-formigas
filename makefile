## TRABALHO DE CG ##

# - VARIABLES

CC = g++
LINLIBS = -lm -lGL -lglut -lGLU
MACLIBS = -framework OpenGL -framework GLUT
CODE = main.cpp
EXE = ./a.out

# - SCRIPT

OS := $(shell uname)
ifeq ($(OS),Darwin)
    all:
	$(CC) $(CODE) -o main $(MACLIBS) -Wno-deprecated-declarations
	$(EXE)
	rm main
else
    all:
	$(CC) -c *.cpp	
	$(CC) *.o $(LINLIBS)
	$(EXE)
	rm *.o
	rm a.out
endif



