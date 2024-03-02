#include <iostream>
#include <cassert>
#include "AnalysisAndVisualization.h"

using namespace std;

int main(int argc, char **argv)
{
    assert(argc == 3 && "Usage: <input file> <output file>");

    string arg1(argv[1]);
    string arg2(argv[2]);
    ifstream input(arg1);
    
    if (!input.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    AnalysisAndVisualization data(arg1);
    data.execute(arg2);
    cout << "Complete!" << endl;
    return 0;
}