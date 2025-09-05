#include <iostream>
#include "HillClimber.h"

// Declare the external evaluator function
double eval(int* solution);


int main() {
    int bestSolution[100];
    int maxNoImprovement = 1000; // You can adjust this value

    // Run hill climber with eval
    hillClimb(eval, bestSolution, maxNoImprovement);
    std::cout << "Best solution (eval): ";
    for (int i = 0; i < 100; ++i) {
        std::cout << bestSolution[i] << " ";
    }
    std::cout << "\nScore: " << eval(bestSolution) << std::endl;

    return 0;
}