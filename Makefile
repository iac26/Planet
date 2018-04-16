# DESTRUCTEUR 2 RADAR
# Author: Iacopo Sprenger
# Version: 1.1
# Date: 25.02.2018
# Filename: Makefile

CFLAGS=-w

LIBS=-framework GLUT -framework OpenGL -framework Cocoa

OFILES=gen.o


all : planet

planet : $(OFILES)
	gcc $(CFLAGS) $(OFILES) -o $@ $(LIBS)
	
gen.o : gen.c
	gcc $(CFLAGS) -c $< -o $@


	
clean : 
	@rm -f *.x *.o
	@echo "CLEANED"
