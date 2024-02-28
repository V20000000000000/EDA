#include "AnalysisAndVisualization.h"

using namespace std;

void AnalysisAndVisualization :: execute(string OutputFile)
{
    DataPreprocessing.parseData(macro, boundary_w, boundary_h, macro_num);
    DataPreprocessing.display(macro, macro_num);
    Plotting.OutputPlot(macro, boundary_w, boundary_h, macro_num, OutputFile);
};
