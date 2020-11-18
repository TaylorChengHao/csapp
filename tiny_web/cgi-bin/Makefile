CC = g++
CSAPP_INC = ../../include
CSAPP_SRC = ../../src

CPPFLAGS = -Wall -O2 -I $(CSAPP_INC) -I . -std=c++11
LDLIBS = -lpthread

all: adder

adder: adder.cpp
	$(CC) $(CPPFLAGS) -o adder adder.cpp

clean:
	rm -f adder *~
