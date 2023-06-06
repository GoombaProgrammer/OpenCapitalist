CPP=g++

# Makefile for OpenCapitalist

OBJS=main.o
CPPFLAGS=-Wall -Wextra -pedantic -std=c++17 -O3 -I./include -I../include
LDFLAGS=-lSDL2 -lSDL2_ttf

all: opencapitalist

opencapitalist: $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o opencapitalist $(LDFLAGS)

main.o: main.cpp
	$(CPP) $(CPPFLAGS) -c main.cpp

clean:
	rm -f $(OBJS) opencapitalist
