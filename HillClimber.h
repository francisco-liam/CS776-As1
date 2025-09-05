#pragma once

void generateInitialSolution(int solution[100]);
void generateNeighborSolution(int solution[100]);
void hillClimb(double (*evaluator)(int*), int bestSolution[100], int maxNoImprovement);