#pragma once

#include <vector>

void setRandomSeed(unsigned int seed);
void generateInitialSolution(int solution[100]);
void generateNeighborSolution(int solution[100]);
void hillClimbWithLog(double (*evaluator)(int*), int bestSolution[100], int maxNoImprovement, std::vector<double>& fitnessLog, unsigned int seed);
void refineWithLog(double (*evaluator)(int*), int solution[100], int maxBitsToSearch, std::vector<double>& fitnessLog);