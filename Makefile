CXX=g++
CXXFLAGS= -std=c++20 -I include -Wall -Wextra -g -lgdi32
LDFLAGS = -lgdi32
all: main
main: block_blast.o
	$(CXX) $(CXXFLAGS) src/main.cpp -o main lib/block_blast.o $(LDFLAGS)
block_blast.o: include/block_blast.h
	$(CXX) $(CXXFLAGS) -c src/block_blast.cpp -o lib/block_blast.o $(LDFLAGS)
clean:
	rm main lib/block_blast.o