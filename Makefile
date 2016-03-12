CC = gcc
CXX = g++
LINK = g++

LIBS = `geant4-config --libs` \
		`root-config --libs` \
		`pkg-config --libs trec` \
		`pkg-config --libs gsl`

CFLAGS =  -Wall -O2 -std=gnu++0x `geant4-config --cflags` \
		`root-config --cflags` \
		`pkg-config --cflags trec` \
		`pkg-config --cflags gsl`

track_reconstruction: main.o
	$(LINK) main.o -o track_reconstruction $(LIBS)

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -f *.o track_reconstruction
