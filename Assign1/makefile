#
# PROGRAM: Lab Training Exercise
# AUTHOR:  Darrius White (Z1900146)
# COURSE:  CSCI 463-01
#
# I certify that this is my own work and where appropriate an extension
# of the starter code provided for the assignment.
#

CXXFLAGS = -Wall -Werror -std=c++11

labtrain: labtrain.o
	g++ $(CXXFLAGS) -o labtrain labtrain.o

labtrain.o: labtrain.cpp
	g++ $(CXXFLAGS) -c labtrain.cpp

clean:
	rm -f *.o labtrain
