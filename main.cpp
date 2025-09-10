#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <filesystem>
#include "HillClimber.h"

// Declare the external evaluator function (black box)
double eval(int* solution);

// Self-made easy evaluator
double evalEasy(int* solution) {
    int correct = 0;
    for (int i = 0; i < 100; ++i) {
        int expected = (i % 2 == 0) ? 0 : 1;
        if (solution[i] == expected) {
            ++correct;
        }
    }
    return static_cast<double>(correct);
}

// Self-made hard evaluator
double evalHard(int* solution) {
    int correct = 0;
    // First 80 bits: even=0, odd=1
    for (int i = 0; i < 80; ++i) {
        int expected = (i % 2 == 0) ? 0 : 1;
        if (solution[i] == expected) {
            ++correct;
        }
    }
    // Last 20 bits: all should be 1
    bool last20all1 = true;
    for (int i = 80; i < 100; ++i) {
        if (solution[i] != 1) {
            last20all1 = false;
            break;
        }
    }
    // Return 100 only if all bits are correct
    if (correct == 80 && last20all1) {
        return 100.0;
    }
    return static_cast<double>(correct);
}

// Helper to write CSV
void write_csv(const std::string& filename, const std::vector<double>& avgFitness) {
    std::ofstream file(filename);
    for (size_t i = 0; i < avgFitness.size(); ++i) {
        file << i << "," << avgFitness[i] << "\n";
    }
    file.close();
}

// Helper to average logs
std::vector<double> average_log(const std::vector<std::vector<double>>& logs) {
    size_t maxlen = 0;
    for (const auto& l : logs) maxlen = std::max(maxlen, l.size());
    std::vector<double> avg(maxlen, 0.0);
    for (size_t i = 0; i < maxlen; ++i) {
        double sum = 0.0;
        int count = 0;
        for (const auto& l : logs) {
            if (i < l.size()) {
                sum += l[i];
                ++count;
            }
        }
        avg[i] = (count > 0) ? (sum / count) : 0.0;
    }
    return avg;
}

int main() {
    int runs = 30;
    int maxNoImprovement = 500;
    int bestSolution[100];

    std::vector<std::vector<double>> logs_blackbox, logs_easy, logs_hard, logs_hard_refine;
    std::vector<unsigned int> seeds;
    std::ifstream seedFileIn("seeds.txt");
    bool seedsExist = seedFileIn.good();

    if (seedsExist) {
        unsigned int seed;
        while (seedFileIn >> seed) {
            seeds.push_back(seed);
        }
        seedFileIn.close();
        if (seeds.size() != runs) {
            std::cerr << "Error: seeds.txt does not contain " << runs << " seeds." << std::endl;
            return 1;
        }
        std::cout << "Read " << seeds.size() << " seeds from seeds.txt\n";
    } else {
        std::ofstream seedFileOut("seeds.txt");
        std::random_device rd;
        for (int i = 0; i < runs; ++i) {
            unsigned int seed = rd();
            seeds.push_back(seed);
            seedFileOut << seed << "\n";
        }
        seedFileOut.close();
        std::cout << "Generated and wrote " << seeds.size() << " seeds to seeds.txt\n";
    }

    // Black box eval
    for (int i = 0; i < runs; ++i) {
        std::vector<double> log;
        setRandomSeed(seeds[i]);
        hillClimbWithLog(eval, bestSolution, maxNoImprovement, log, seeds[i]);
        logs_blackbox.push_back(log);
    }

    // Easy eval
    for (int i = 0; i < runs; ++i) {
        std::vector<double> log;
        setRandomSeed(seeds[i]);
        hillClimbWithLog(evalEasy, bestSolution, maxNoImprovement, log, seeds[i]);
        logs_easy.push_back(log);
    }

    // Hard eval
    for (int i = 0; i < runs; ++i) {
        std::vector<double> log;
        setRandomSeed(seeds[i]);
        hillClimbWithLog(evalHard, bestSolution, maxNoImprovement, log, seeds[i]);
        logs_hard.push_back(log);
    }

    // --- CSV naming based on black box type ---
    #ifdef BLACKBOX_EASY
        std::string blackbox_suffix = "_blackbox_easy";
    #elif defined(BLACKBOX_HARD)
        std::string blackbox_suffix = "_blackbox_hard";
    #else
        std::string blackbox_suffix = "_blackbox";
    #endif

    write_csv("hillclimber" + blackbox_suffix + ".csv", average_log(logs_blackbox));
    write_csv("hillclimber_easy.csv", average_log(logs_easy));
    write_csv("hillclimber_hard.csv", average_log(logs_hard));

    // --- Report average number of iterations per problem ---
    auto avg_iters = [](const std::vector<std::vector<double>>& logs) {
        double sum = 0.0;
        for (const auto& log : logs) sum += log.size();
        return (logs.empty()) ? 0.0 : (sum / logs.size());
    };

    std::cout << "Average # iterations (blackbox): " << avg_iters(logs_blackbox) << std::endl;
    std::cout << "Average # iterations (easy):     " << avg_iters(logs_easy) << std::endl;
    std::cout << "Average # iterations (hard):     " << avg_iters(logs_hard) << std::endl;

    // // Only refine and write the refinement CSV if the black box is hard
    // #ifdef BLACKBOX_HARD
    //     logs_hard_refine.clear();
    //     for (int i = 0; i < runs; ++i) {
    //         std::vector<double> refineLog;
    //         setRandomSeed(seeds[i]);
    //         hillClimbWithLog(eval, bestSolution, maxNoImprovement, refineLog, seeds[i]);
    //         setRandomSeed(seeds[i]);
    //         refineWithLog(eval, bestSolution, 30, refineLog);
    //         logs_hard_refine.push_back(refineLog);
    //     }
    //     write_csv("hillclimber_blackbox_hard_refine.csv", average_log(logs_hard_refine));
    // #endif

    // // Always refine and write the refinement CSV for the self-made hard eval
    // logs_hard_refine.clear();
    // for (int i = 0; i < runs; ++i) {
    //     std::vector<double> refineLog;
    //     setRandomSeed(seeds[i]);
    //     hillClimbWithLog(evalHard, bestSolution, maxNoImprovement, refineLog, seeds[i]);
    //     setRandomSeed(seeds[i]);
    //     refineWithLog(evalHard, bestSolution, 30, refineLog);
    //     logs_hard_refine.push_back(refineLog);
    // }
    // write_csv("hillclimber_hard_refine.csv", average_log(logs_hard_refine));

    std::cout << "Experiments complete. CSVs and seeds.txt written." << std::endl;
    return 0;
}