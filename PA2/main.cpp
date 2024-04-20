#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "InputDataParse.hpp"
#include "HorizontalGraph.hpp"
#include "InitialSolution.hpp"
#include "simulated_annealing.hpp"
#include "Timer.hpp"

using namespace std;

int main(int argc, char **argv)
{
    assert(argc == 3 && "Usage: <input file> <output file>");

    string arg1(argv[1]);
    string arg2(argv[2]);
    ifstream inputFile(arg1);
    
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    inputFile.close();

    InitialSolution initialSolution(arg1);

    ofstream outputFile(arg2);

    outputFile.close();

    cout << "Complete1!" << endl;

    return 0;
}