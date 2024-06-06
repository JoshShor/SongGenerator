#ifndef SONGGEN_H
#define SONGGEN_H

#include <string>
#include <vector>
#include <tuple>

const int POPULATION_SIZE = 30;
const double MUTATION_RATE = 0.1;
const int MELODY_LENGTH = 10;

std::string createMelody(int melody_length);
std::tuple<std::string, std::string, std::string, std::string> crossover(std::string& mel_1, std::string& mel_2);
void mutate(std::string& melody);
int fitnessFunction(std::string& melody);
std::tuple<std::string, std::string> chooseParents(std::string& m1, std::string& m2, std::string& m3, std::string& m4);

#endif /* SONGGEN_H */
