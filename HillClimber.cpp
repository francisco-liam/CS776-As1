#include "HillClimber.h"
#include <random>

void generateInitialSolution(int solution[100]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    for (int i = 0; i < 100; ++i) {
        solution[i] = dist(gen);
    }
}

void generateNeighborSolution(int solution[100]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> indexDist(0, 99);

    int idx = indexDist(gen);
    solution[idx] = 1 - solution[idx]; // Flip the bit (0->1, 1->0)
}

// Hill climber: tries to maximize the evaluator function
void hillClimb(double (*evaluator)(int*), int bestSolution[100], int maxNoImprovement) {
    int currentSolution[100];
    generateInitialSolution(currentSolution);
    double bestScore = evaluator(currentSolution);
    std::copy(currentSolution, currentSolution + 100, bestSolution);

    int noImprovement = 0;
    while (noImprovement < maxNoImprovement) {
        int neighbor[100];
        std::copy(currentSolution, currentSolution + 100, neighbor);
        generateNeighborSolution(neighbor);

        double neighborScore = evaluator(neighbor);
        if (neighborScore >= bestScore) {
            bestScore = neighborScore;
            std::copy(neighbor, neighbor + 100, currentSolution);
            std::copy(neighbor, neighbor + 100, bestSolution);
            noImprovement = 0;
        } else {
            ++noImprovement;
        }
    }
}