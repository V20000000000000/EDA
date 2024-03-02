#ifndef ANALYSISANDVISUALIZATION
#define ANALYSISANDVISUALIZATION

#include "DataPreprocess.h"
#include "GeneratePlot.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class AnalysisAndVisualization
{
    private:
        vector<vector<int>> macro;
        vector<string> macro_label;
        int boundary_w, boundary_h;
        int macro_num;
        
        DataPreprocess DataPreprocessing;
        GeneratePlot Plotting;
    public:
        AnalysisAndVisualization(){};
        AnalysisAndVisualization(string argument) : DataPreprocessing(argument){};
        void execute(string OutputFile);
};

#endif