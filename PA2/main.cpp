#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <tuple>
#include <set>
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

    //---------------------------------------------
    
    HVGraph<Block, int> horizontalGraph = initialSolution.getHorizontalGraph();
    HVGraph<Block, int> verticalGraph = initialSolution.getVerticalGraph();
    set<int> sourceSetH = horizontalGraph.getSourceSet();
    set<int> sourceSetV = verticalGraph.getSourceSet();
    set<int> TargetSetH = horizontalGraph.getTargetSet();
    set<int> TargetSetV = verticalGraph.getTargetSet();

    float chipWidth, chipHeight;
    int Htarget = 0;
    int Vtarget = 0;

    cout << horizontalGraph.size() << endl;
    //-------------------------------------
    // cout << "sourceSet" << endl;
    // for (auto i : sourceSetH)
    // {
    //     cout << i << " ";
    // }
    // cout << endl;
    //------------------------------
    // std::tie(chipWidth, std::ignore, Htarget) = horizontalGraph.findMaxDistance(sourceSetH, TargetSetH);
    // std::tie(chipHeight, std::ignore, Vtarget) = verticalGraph.findMaxDistance(sourceSetV, TargetSetV);
    // cout << "Chip width: " << chipWidth + horizontalGraph.getVertexProperty(Htarget).value.getWidth() << endl;
    // cout << "Chip height: " << chipHeight + verticalGraph.getVertexProperty(Vtarget).value.getHeight() << endl;

    //GenerateOutput generateOutput(horizontalGraph, verticalGraph);
    cout << arg2 << endl;
    //generateOutput.generateOutputFile(arg2);
    //generateOutput.generatePA1inputFile();

    cout << "Complete!" << endl;
    cout << "Time: " << timer.stop() << " ms" << endl;

    return 0;
}