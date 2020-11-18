CC = g++

CSAPP_INC = ../include
CSAPP_SRC = ../src

CPPFLAGS = -Wall -O2 -I $(CSAPP_INC) -I . -std=c++11
LDLIBS = -lpthread

PROGS =	echoservers\

all: $(CSAPP_SRC)/csapp.o $(PROGS) 
$(PROGS): $(CSAPP_SRC)/csapp.o
$(CSAPP_SRC)/csapp.o: $(CSAPP_SRC)/csapp.cpp $(CSAPP_INC)/csapp.h

clean:
	rm -f $(PROGS) *.o *~
