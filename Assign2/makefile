#
# PROGRAM: prog2
# AUTHOR:  Darrius White (Z1900146)
# COURSE:  CSCI 463-01
#
# I certify that this is my own work and where appropriate an extension
# of the starter code provided for the assignment.
#

CXXFLAGS = -Wall -Werror -std=c++11

prog2: prog2.o
	g++ $(CXXFLAGS) -o prog2 prog2.o

prog2.o: prog2.cpp
	g++ $(CXXFLAGS) -c prog2.cpp

clean:
	rm -f *.o prog2
