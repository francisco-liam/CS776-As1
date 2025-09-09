CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

all: hillclimber_easy hillclimber_hard

hillclimber_easy: main_easy.o HillClimber.o evalEasy.o
	$(CXX) $(CXXFLAGS) -o hillclimber_easy main_easy.o HillClimber.o evalEasy.o

hillclimber_hard: main_hard.o HillClimber.o evalHard.o
	$(CXX) $(CXXFLAGS) -o hillclimber_hard main_hard.o HillClimber.o evalHard.o

main_easy.o: main.cpp HillClimber.h
	$(CXX) $(CXXFLAGS) -DBLACKBOX_EASY -c main.cpp -o main_easy.o

main_hard.o: main.cpp HillClimber.h
	$(CXX) $(CXXFLAGS) -DBLACKBOX_HARD -c main.cpp -o main_hard.o

HillClimber.o: HillClimber.cpp HillClimber.h
	$(CXX) $(CXXFLAGS) -c HillClimber.cpp

.PHONY: clean
clean:
	rm -f main_easy.o main_hard.o HillClimber.o evalSelfEasy.o hillclimber_easy hillclimber_hard