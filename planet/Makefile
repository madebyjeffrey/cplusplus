# Makefile for "planet", a fractal planet generator

# Change the following if your C compiler isn't called 'gcc'.
CC=clang++

# And change this to your favourite C compiler flags:
CFLAGS = -g -W -Wall -ferror-limit=10000 -Wno-unused-variable 
CFLAGS += `pkg-config --cflags tclap`


OBJS = planet.o image_bmp.o image_ppm.o image_xpm.o planet_fn.o projections.o

LIBS = -lm `pkg-config --libs tclap`

.cpp.o:
	$(CC) -c $(CFLAGS) $*.cpp

all:	planet

planet: $(OBJS)
	$(CC) $(CFLAGS) -o planet $(OBJS) $(LIBS)
	@echo "planet made"

clean:
	rm -f $(OBJS) planet

SHARFILES = Manual.txt Makefile ReadMe \
            planet.cpp \
            default.col defaultB.col burrows.col burrowsB.col mars.col\
            wood.col white.col

zip:	$(SHARFILES)
	zip planet.zip $(SHARFILES)

rmarc:
	rm -f planet.zip
