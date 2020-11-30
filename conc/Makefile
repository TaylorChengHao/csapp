CC = g++

CSAPP_INC = ../include
CSAPP_SRC = ../src

CPPFLAGS = -Wall -O2 -I $(CSAPP_INC) -I . -std=c++11
LDLIBS = -lpthread

PROGS =	echo.o\
		echo_cnt.o\
		echoservers\
		echoservert\
		select\
		echoservert_pre\

all: $(CSAPP_SRC)/csapp.o $(PROGS) 
$(PROGS): $(CSAPP_SRC)/csapp.o
$(CSAPP_SRC)/csapp.o: $(CSAPP_SRC)/csapp.cpp $(CSAPP_INC)/csapp.h

select: select.o echo.o

echoservert: echoservert.o echo.o

echoservert_pre: echoservert_pre.o echo_cnt.o sbuf.o

clean:
	rm -f $(PROGS) *.o *~
