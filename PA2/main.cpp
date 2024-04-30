#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <tuple>
#include <set>
#include <vector>
#include "InputDataParse.hpp"
#include "HVGraph.hpp"
#include "InitialSolution.hpp"
#include "Timer.hpp"
#include "GenerateOutput.hpp"
#include "SimulatedAnnealing.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Timer timer;
    timer.start();

    // assert(argc == 3 && "Usage: <input file> <output file>");

    // convert argv to string
    string arg1 = argv[1];
    string arg2 = argv[2];

    cout << "Input file: " << arg1 << endl;
    cout << "Output file: " << arg2 << endl;

    ifstream inputFile(arg1);
    if (!inputFile.is_open())
    {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    inputFile.close();

    cout << "initialSolution" << endl;
    // Process input file
    InitialSolution initialSolution(arg1);
    vector<string> blockNameList = initialSolution.getBlockNameList();
    cout << "finish initialSolution" << endl;

    //---------------------------------------------

    HVGraph<Block *, int> *horizontalGraph = initialSolution.getHorizontalGraph();
    HVGraph<Block *, int> *verticalGraph = initialSolution.getVerticalGraph();

    // GenerateOutput generateOutput(horizontalGraph, verticalGraph);
    // cout << arg2 << endl;
    // generateOutput.generateOutputFile(arg2);
    // generateOutput.generatePA1inputFile();

    //---------------------------------------------

    // Simulated Annealing
    cout << "Simulated Annealing Start" << endl;
    SimulatedAnnealing simulatedAnnealing(horizontalGraph, verticalGraph);
    simulatedAnnealing.run();
    cout << "Simulated Annealing End" << endl;

    // HVGraph<Block *, int> *hGraph = simulatedAnnealing.getHorizontalGraph();
    // HVGraph<Block *, int> *vGraph = simulatedAnnealing.getVerticalGraph();

    // float chipWidth, chipHeight;

    //---------------------------------------------

    // set<int> a = horizontalGraph->getSourceSet();

    // cout << "SourceSet: " << endl;
    // for (auto i : a)
    // {
    //     cout << i << " ";
    // }
    // cout << endl;

    // cout << string(50, '-') << endl;
    // cout << "Print HGraph" << endl;
    // horizontalGraph->printGraph();
    // cout << "Print VGraph" << endl;
    // verticalGraph->printGraph();
    // cout << string(50, '-') << endl;

    // rotate the 7th block
    // horizontalGraph->rotateBlock(6, false);
    // verticalGraph->rotateBlock(6, true);

    // cout << string(50, '-') << endl;
    // cout << "Print HGraph" << endl;
    // horizontalGraph->printGraph();
    // cout << "Print VGraph" << endl;
    // verticalGraph->printGraph();
    // cout << string(50, '-') << endl;

    cout << "Generate Output" << endl;

    GenerateOutput generateOutput(horizontalGraph, verticalGraph);
    cout << arg2 << endl;
    generateOutput.generateOutputFile(arg2, blockNameList, simulatedAnnealing.getCoordinateX(), 
    simulatedAnnealing.getCoordinateY(), simulatedAnnealing.getGlobalBestH(), simulatedAnnealing.getGlobalBestV() 
    );



    cout << "Complete!" << endl;
    cout << "Time: " << timer.stop() << " ms" << endl;

    return 0;
}