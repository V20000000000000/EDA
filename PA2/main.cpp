#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <tuple>
#include "InputDataParse.hpp"
#include "HVGraph.hpp"
#include "InitialSolution.hpp"
#include "Timer.hpp"
#include "GenerateOutput.hpp"

using namespace std;

int main(int argc, char **argv)
{
    Timer timer;
    timer.start();

    assert(argc == 3 && "Usage: <input file> <output file>");

    string arg1(argv[1]);
    string arg2(argv[2]);
    
    ifstream inputFile(arg1);
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    inputFile.close();

    // Process input file
    InitialSolution initialSolution(arg1);
    HVGraph<Block, int> horizontalGraph = initialSolution.getHorizontalGraph();
    HVGraph<Block, int> verticalGraph = initialSolution.getVerticalGraph();
     float chipWidth, chipHeight;
     std::tie(chipWidth, std::ignore, std::ignore) = horizontalGraph.findMaxDistance(0);
     std::tie(chipHeight, std::ignore, std::ignore) = verticalGraph.findMaxDistance(1);
    cout << "Chip width: " << chipWidth << endl;
    cout << "Chip height: " << chipHeight << endl;

    GenerateOutput generateOutput(horizontalGraph, verticalGraph);
    cout << arg2 << endl;
    generateOutput.generateOutputFile(arg2);
    generateOutput.generatePA1inputFile();

    cout << "Complete!" << endl;
    cout << "Time: " << timer.stop() << " ms" << endl;

    return 0;
}