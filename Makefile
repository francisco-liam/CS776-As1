CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

all: hillclimber_easy hillclimber_hard

hillclimber_easy: main.o HillClimber.o evalEasy.o
	$(CXX) $(CXXFLAGS) -o hillclimber_easy main.o HillClimber.o evalEasy.o

hillclimber_hard: main.o HillClimber.o evalHard.o
	$(CXX) $(CXXFLAGS) -o hillclimber_hard main.o HillClimber.o evalHard.o

main.o: main.cpp HillClimber.h
	$(CXX) $(CXXFLAGS) -c main.cpp

HillClimber.o: HillClimber.cpp HillClimber.h
	$(CXX) $(CXXFLAGS) -c HillClimber.cpp

clean:
	rm -f main.o HillClimber.o hillclimber_easy hillclimber_hard