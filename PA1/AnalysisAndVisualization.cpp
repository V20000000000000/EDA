#include "AnalysisAndVisualization.h"

using namespace std;

void AnalysisAndVisualization :: execute(string OutputFile)
{
    DataPreprocessing.parseData(macro, macro_label, boundary_w, boundary_h, macro_num);
    //DataPreprocessing.display(macro, macro_label, macro_num);
    Plotting.outputPlot(macro, macro_label, boundary_w, boundary_h, macro_num, OutputFile);
};
