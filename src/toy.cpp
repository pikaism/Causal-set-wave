#include <iostream>
#include <vector>
#include <fstream>
#include <random>

int main() {
    // 1. A vector is a resizable array. Here it will hold random doubles.
    std::vector<double> numbers;

    // 2. Random number setup (you'll use this exact pattern for Poisson sprinkling tomorrow)
    std::mt19937 rng(42);                              // random engine, seeded for reproducibility
    std::uniform_real_distribution<double> dist(0.0, 1.0); // uniform numbers between 0 and 1

    // 3. Fill the vector with 10 random numbers
    int N = 10;
    for (int i = 0; i < N; i++) {
        double x = dist(rng);
        numbers.push_back(x);   // push_back adds an element to the end of the vector
    }

    // 4. Write the vector to a file
    std::ofstream outFile("data/toy_output.txt"); // opens file for writing
    for (double x : numbers) {                    // range-based for loop: "for each x in numbers"
        outFile << x << "\n";
    }
    outFile.close();
    std::cout << "Wrote " << numbers.size() << " numbers to data/toy_output.txt" << std::endl;

    // 5. Read the file back into a new vector, to confirm round-tripping works
    std::vector<double> readBack;
    std::ifstream inFile("data/toy_output.txt");
    double value;
    while (inFile >> value) {       // reads one number at a time until the file ends
        readBack.push_back(value);
    }
    inFile.close();

    std::cout << "Read back " << readBack.size() << " numbers:" << std::endl;
    for (double x : readBack) {
        std::cout << "  " << x << std::endl;
    }

    return 0;
}