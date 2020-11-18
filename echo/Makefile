CC = g++

CSAPP_INC = ../include
CSAPP_SRC = ../src

CPPFLAGS = -Wall -O2 -I $(CSAPP_INC) -I . -std=c++11
LDLIBS = -lpthread

PROGS =	echo.o\
	test\
	client\
	server\

all: $(CSAPP_SRC)/csapp.o $(PROGS) 
$(PROGS): $(CSAPP_SRC)/csapp.o
$(CSAPP_SRC)/csapp.o: $(CSAPP_SRC)/csapp.cpp $(CSAPP_INC)/csapp.h

server: server.o echo.o

clean:
	rm -f $(PROGS) *.o *~
