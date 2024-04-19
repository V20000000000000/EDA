#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include "InputDataParse.hpp"

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

    InputDataParse parser(arg1);

    parser.getNumBlocks();
    parser.getMinAspectRatio();
    parser.getMaxAspectRatio();
    parser.getBlocksList();

    parser.getBlock(34);
    parser.getBlockWidth(34);
    parser.getBlockHeight(34);

    ofstream outputFile(arg2);

    outputFile.close();

    cout << "Complete!" << endl;
    return 0;
}