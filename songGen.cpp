#include "songGen.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

char octave[] = {'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G', 'A', 'A', 'B'};
//char octave[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
char accidentals[] = {' ', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', '#', ' '}; //not used. Will use for future.

// Generator
std::string createMelody(int melody_length) {
    int k = 0;
    std::string melody = "";

    for (int i = 0; i < melody_length; i++) {
        k = rand() % 12;
        melody += octave[k];
    }
    return melody;
}

// Crossover function
std::tuple<std::string, std::string, std::string, std::string> crossover(std::string& mel_1, std::string& mel_2) {
	// Check if either melody is empty
    if (mel_1.empty()) {
        // If mel_1 is empty, generate a new melody based on mel_2
        std::string new_mel_1 = createMelody(MELODY_LENGTH);
        mel_1 = new_mel_1;
    }
    if (mel_2.empty()) {
		// If mel_2 is empty, generate a new melody based on mel_1
        std::string new_mel_2 = createMelody(MELODY_LENGTH);
        mel_2 = new_mel_2;
    }
    // Calculate the midpoint of the melodies
    int mid = mel_1.length() / 2;

    std::string seg1 = mel_1.substr(0, mid);
    std::string seg2 = mel_2.substr(0, mid);
    std::string seg3 = mel_1.substr(mid);
    std::string seg4 = mel_2.substr(mid);

    //cout << "1: " << seg1 << " 2: " << seg2 << " 3: " << seg3 << " 4: " << seg4 << endl; 

    // Perform crossover
    std::string child1 = seg1 + seg4;
    std::string child2 = seg2 + seg3;
    std::string child3 = seg3 + seg2;
    std::string child4 = seg4 + seg1;

    //cout << " child 1: " << child1 << " child 2: " << child2 << " child 3: " << child3 << " child 4: " << child4 << endl;

    return std::make_tuple(child1, child2, child3, child4);
}

// Mutation function
void mutate(std::string& melody) {
    for (int i = 0; i < melody.length(); i++) {
        if (rand() < RAND_MAX * MUTATION_RATE) {
            int note_index = rand() % 12;
            melody[i] = octave[note_index];
        }
    }
}

// Function to check if a melody has a wave-shaped contour
// Funtion called within fitnessFunction.
bool hasWaveShapedContour(const std::string& melody) {
	
	// Check if the contour goes up then down then up again, or vice versa
    bool increasing = melody[1] > melody[0];
    bool decreasing = melody[2] < melody[1];

    for (size_t i = 3; i < melody.size(); ++i) {
        if ((melody[i] > melody[i - 1] && decreasing) || (melody[i] < melody[i - 1] && increasing)) {
			// Contour direction changes, so it's a wave-shaped contour
            return true;
        }

        if (melody[i] > melody[i - 1]) {
            increasing = true;
            decreasing = false;
        } else if (melody[i] < melody[i - 1]) {
            increasing = false;
            decreasing = true;
        }
    }
	// If the function reaches here, no wave-shaped contour was found
    return false;
}

// Fitness function with repetition, variation, and wave-shaped contour
int fitnessFunction(std::string& melody) {
    const int NUM_NOTES = 12; // Number of different notes
    int noteCounts[NUM_NOTES] = {0}; // Array to store counts of each note
    int repeatedNotesCount = 0; // To count repeated notes
    int uniqueNotesCount = 0; // To count unique notes

    //if its empty then set score to 0
    if(melody.empty()){
        return 0;
    }

	// Iterate through the melody to count repeated notes and collect unique notes
    for (char note : melody) {
        int index = 0;
        for(int i = 0; i < 12; i++){
            if(note == octave[i]){
                index = i;
                break;
            }
        }
        noteCounts[index]++; // Increment count for the corresponding note
        if (noteCounts[index] == 2) {
            repeatedNotesCount++; // Increment repeated notes count if the note is repeated
        } else if (noteCounts[index] == 1) {
            uniqueNotesCount++; // Increment unique notes count if it's the first occurrence
        }
    }

    // Check for wave-shaped contour
    bool hasWaveContour = hasWaveShapedContour(melody);

    // Calculate variation score
    double variationScore = 1.0 - static_cast<double>(repeatedNotesCount) / melody.size();

    // Calculate fitness score
    double fitnessScore = variationScore * 10;

    // If melody does not have a wave-shaped contour, reduce fitness score
    if (!hasWaveContour) {
        fitnessScore *= 0.5; // Penalize the fitness score for not having a wave-shaped contour
    }

    return static_cast<int>(fitnessScore);
}

std::tuple<std::string, std::string> chooseParents(std::string& m1, std::string& m2, std::string& m3, std::string& m4) {
    std::string mel1, mel2;
    int choice1, choice2;

    int f1 = fitnessFunction(m1);
    int f2 = fitnessFunction(m2);
    int f3 = fitnessFunction(m3);
    int f4 = fitnessFunction(m4);
    int arr[] = {f1, f2, f3, f4};
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    std::sort(arr, arr + arrSize, std::greater<>());

    choice1 = arr[0];
    choice2 = arr[1];

    if (choice1 == f1 || choice2 == f1) {
        mel1 = m1;
    } else if (choice1 == f2 || choice2 == f2) {
        mel1 = m2;
    }

    if (choice1 == f3 || choice2 == f3) {
        mel2 = m3;
    } else if (choice1 == f4 || choice2 == f4) {
        mel2 = m4;
    }
	
	//cout << "CHOSEN CHILD 1 " << mel1 << endl << "CHOSEN CHILD 2 " << mel2 << endl;
    return std::make_tuple(mel1, mel2);
}
