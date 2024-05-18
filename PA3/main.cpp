#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>

#include "InputDataParse.hpp"
#include "Timer.hpp"

using namespace std;

int main( int argc, char **argv )
{
    Timer timer;
    timer.start();

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

    ofstream outputFile(arg2);

    InputDataParse inputDataParse;
    inputDataParse.parseInputData(arg1);
    inputDataParse.printParsedData();

    timer.stop();
    cout << "Time elapsed: " << timer.elapsed() << " seconds." << endl;

    return 0;

}