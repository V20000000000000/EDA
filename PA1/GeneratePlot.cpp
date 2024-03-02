#include "GeneratePlot.h"

GeneratePlot :: GeneratePlot()
{

};

GeneratePlot :: GeneratePlot(vector<vector<int>> &macro, vector<string> &macro_label, int &boundary_w, int &boundary_h, int &macro_num)
{
    macro = macro;
    macro_label = macro_label;
    boundary_w = boundary_w;
    boundary_h = boundary_h;
    macro_num = macro_num;
};

void GeneratePlot :: outputPlot(vector<vector<int>> &macro, vector<string> &macro_label, int &boundary_w, int &boundary_h, int &macro_num, string OutputFile)
{
    ifstream input_prefix("prefix.txt");
    ifstream input_suffix("suffix.txt");
    ofstream outputFile(OutputFile);
    string str;
    int xtics, ytics;

    //input test
    if (!input_prefix.is_open()) 
    {
        cout << "Error: Failed to open the prefix.txt." << endl;
        return;
    }
    else if(!input_suffix.is_open()) 
    {
        cout << "Error: Failed to open the suffix.txt." << endl;
        return;
    }

    //write prefix
    while (getline(input_prefix, str))
    {
        outputFile << str << endl;
    }

    //write command
    for(int i = 0; i < macro_num; i++)
    {
        int center_x, center_y;
        center_x = (macro[i][0] + macro[i][2]) / 2;
        center_y = (macro[i][1] + macro[i][3]) / 2;
        //cout << "set object " << i+1 << " rect from " << macro[i][0] << "," << macro[i][1] << " to " << macro[i][2] << "," << macro[i][3] << endl;
        //cout << "set label \"" << macro_label[i] << "\" at " << center_x << "," << center_y << " center" << endl; 
        outputFile << "set object " << i+1 << " rect from " << macro[i][0] << "," << macro[i][1] << " to " << macro[i][2] << "," << macro[i][3] << endl;
        outputFile << "set label \"" << macro_label[i] << "\" at " << center_x << "," << center_y << " center" << endl; 
    }
    
    //write set_tics
        generateTics(boundary_w, boundary_h, xtics, ytics);
        outputFile << "set xtics " << xtics << endl;
        outputFile << "set ytics " << ytics << endl;

    //write plot [0:boundary_][0:boundary_h]0
    outputFile << "plot" << " " << "[0:" << boundary_w << "]" << "[0:" << boundary_h << "]" << "0" << endl;

    //write suffix
    while (getline(input_suffix, str))
    {
        outputFile << str << endl;
    }
};

void GeneratePlot :: generateTics(int boundary_w, int boundary_h, int &xtics, int &ytics)
{
    if(boundary_w > 5)
        {
            xtics = boundary_w / 5;
        }else{
            xtics = 1;
        }
        if(boundary_h > 5)
        {
            ytics = boundary_h / 5;
        }else{
            ytics = 1;
        }
}; 

