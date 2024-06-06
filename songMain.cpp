#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "songgen.h"

int main() {
    srand(static_cast<unsigned>(time(0))); // Initialize random number generator

    int targetFitness = 9; // Target fitness score
    int generation = 0; // Current generation

    int bestFitness = 0; // Fitness score of the best melody
    std::string bestMelody; // Best melody found so far

    // Generate initial melodies
    std::string melody1 = createMelody(MELODY_LENGTH);
    std::string melody2 = createMelody(MELODY_LENGTH);

    // Add initial melodies to the population
    /* 
	vector<string> population;
    population.push_back(melody1);
    population.push_back(melody2);
	*/

    // set parents from the initial melodies
    std::string parent1 = melody1;
    std::string parent2 = melody2;

    std::cout << "Initial generation: \n";

    //cout << melody1 << " " << melody2 << endl;
 
    // Continue generating melodies until the target fitness score is reached
    while (bestFitness < targetFitness) {
        // Select parents, perform crossover, and mutate
        for (int i = 0; i < POPULATION_SIZE; i += 2) {
			
            //select parents randomly /// NOT USED ANYMORE
			/*
            int parent1Index = rand() % population.size();
            int parent2Index = rand() % population.size();

            // Add parents to the population
            population.push_back(parent1);
            population.push_back(parent2);
			*/
            int fitness1 = fitnessFunction(parent1);
            int fitness2 = fitnessFunction(parent2);

            // Perform crossover
            auto [child1, child2, child3, child4] = crossover(parent1, parent2);

            // Mutate children
            mutate(child1);
            mutate(child2);
            mutate(child3);
            mutate(child4);

            std::cout << "Parent 1: " << parent1 << " with fitness score: " << fitness1 << std::endl;
            std::cout << "Parent 2: " << parent2 << " with fitness score: " << fitness2 << std::endl;
            std::cout << std::endl;
            std::cout << "Child 1: " << child1 << std::endl;
            std::cout << "Child 2: " << child2 << std::endl;
            std::cout << "Child 3: " << child3 << std::endl;
            std::cout << "Child 4: " << child4 << std::endl;
            std::cout << std::endl;

            // Choose parents based on fitness
            auto [p1, p2] = chooseParents(child1, child2, child3, child4);
			
            // std::cout << "ch1 : " << chic1 << " || " << "ch2 : " << chic2 << std::endl;

            parent1 = p1;
            parent2 = p2;

            // Evaluate fitness of children and update best melody
            fitness1 = fitnessFunction(parent1);
            fitness2 = fitnessFunction(parent2);

            if (fitness1 > bestFitness) {
                bestMelody = parent1;
                bestFitness = fitness1;
            } else if (fitness2 > bestFitness) {
                bestMelody = parent2;
                bestFitness = fitness2;
            }

            // Count the generation
				generation++;

            // Once it hits target fitness score, break out of for-loop
            if (fitness1 == 9 || fitness2 == 9) {
                break;
            }
			//std::cout << "bestFitness " << bestFitness << std::endl;
        }
    }
    std::cout << "Target fitness reached at generation #" << generation << std::endl;
    std::cout << "Best Melody: " << bestMelody << std::endl;
    std::cout << "Fitness Score: " << bestFitness << std::endl;

    return 0;
}
