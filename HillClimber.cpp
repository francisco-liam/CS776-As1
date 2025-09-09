#include "HillClimber.h"
#include <random>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm> // For std::shuffle

// Shared random engine
static std::mt19937 gen;

// Set the random seed for reproducibility
void setRandomSeed(unsigned int seed) {
    gen.seed(seed);
}

void generateInitialSolution(int solution[100]) {
    std::uniform_int_distribution<> dist(0, 1);
    for (int i = 0; i < 100; ++i) {
        solution[i] = dist(gen);
    }
}

void generateNeighborSolution(int solution[100]) {
    std::uniform_int_distribution<> indexDist(0, 99);
    int idx = indexDist(gen);
    solution[idx] = 1 - solution[idx]; // Flip the bit (0->1, 1->0)
}

// Hill climber with logging: records best fitness at each iteration
void hillClimbWithLog(double (*evaluator)(int*), int bestSolution[100], int maxNoImprovement, std::vector<double>& fitnessLog, unsigned int seed) {
    setRandomSeed(seed);
    int currentSolution[100];
    generateInitialSolution(currentSolution);
    double bestScore = evaluator(currentSolution);
    std::copy(currentSolution, currentSolution + 100, bestSolution);

    int noImprovement = 0;
    fitnessLog.clear();
    fitnessLog.push_back(bestScore);

    while (noImprovement < maxNoImprovement) {
        int neighbor[100];
        std::copy(currentSolution, currentSolution + 100, neighbor);
        generateNeighborSolution(neighbor);

        double neighborScore = evaluator(neighbor);
        if (neighborScore > bestScore) {
            bestScore = neighborScore;
            std::copy(neighbor, neighbor + 100, currentSolution);
            std::copy(neighbor, neighbor + 100, bestSolution);
            noImprovement = 0;
        } else {
            ++noImprovement;
        }
        fitnessLog.push_back(bestScore);
        //if (bestScore == 100.0) break;
    }
}

// Refinement with logging: records fitness at each refinement iteration
void refineWithLog(double (*evaluator)(int*), int solution[100], int maxBitsToSearch, std::vector<double>& fitnessLog) {
    double baseScore = evaluator(solution);
    std::vector<int> unlockedBits;

    for (int i = 0; i < 100; ++i) {
        solution[i] = 1 - solution[i];
        double newScore = evaluator(solution);
        if (newScore < baseScore) {
            solution[i] = 1 - solution[i];
        } else {
            solution[i] = 1 - solution[i];
            unlockedBits.push_back(i);
        }
    }

    size_t numUnlocked = unlockedBits.size();
    if (numUnlocked > static_cast<size_t>(maxBitsToSearch)) {
        std::cout << "Too many bits to exhaustively search (" << numUnlocked << ")." << std::endl;
        return;
    }

    int bestCandidate[100];
    std::copy(solution, solution + 100, bestCandidate);
    double bestScore = evaluator(solution);

    size_t totalCombinations = 1ULL << numUnlocked;
    // Prepare all possible masks
    std::vector<size_t> masks(totalCombinations);
    for (size_t i = 0; i < totalCombinations; ++i) {
        masks[i] = i;
    }
    // Shuffle the order randomly
    std::shuffle(masks.begin(), masks.end(), gen);

    fitnessLog.clear();
    fitnessLog.push_back(bestScore);

    for (size_t idx = 0; idx < masks.size(); ++idx) {
        size_t mask = masks[idx];
        int candidate[100];
        std::copy(solution, solution + 100, candidate);
        for (size_t j = 0; j < numUnlocked; ++j) {
            int bitIdx = unlockedBits[j];
            if (mask & (1ULL << j)) {
                candidate[bitIdx] = 1 - candidate[bitIdx];
            }
        }
        double score = evaluator(candidate);
        fitnessLog.push_back(score);
        if (score > bestScore) {
            bestScore = score;
            std::copy(candidate, candidate + 100, bestCandidate);
            if (bestScore == 100.0) {
                std::copy(bestCandidate, bestCandidate + 100, solution);
                return;
            }
        }
    }
    std::copy(bestCandidate, bestCandidate + 100, solution);
}