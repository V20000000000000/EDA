#ifndef GENERATEPLOT_H
#define GENERATEPLOT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class GeneratePlot
{
    private:
        vector<vector<int>> macro;
        vector<int> macro_label;
        int boundary_w, boundary_h;
        int macro_num;
    public:
        GeneratePlot();
        GeneratePlot(vector<vector<int>> &macro, vector<string> &macro_label, int &boundary_w, int &boundary_h, int &macro_num);
        void outputPlot(vector<vector<int>> &macro, vector<string> &macro_label, int &boundary_w, int &boundary_h, int &macro_num, string outputFile);
        void generateTics(int boundary_w, int boundary_h, int &xtics, int &ytics);
};

#endif