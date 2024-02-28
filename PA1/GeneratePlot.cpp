#include "GeneratePlot.h"

GeneratePlot :: GeneratePlot()
{

};

GeneratePlot :: GeneratePlot(vector<vector<int>> &macro,int &boundary_w, int &boundary_h, int &macro_num)
{
    macro = macro;
    boundary_w = boundary_w;
    boundary_h = boundary_h;
    macro_num = macro_num;
};

void GeneratePlot :: OutputPlot(vector<vector<int>> &macro,int &boundary_w, int &boundary_h, int &macro_num, string OutputFile)
{
    ifstream input_prefix("prefix.txt");
    ifstream input_set_tics("set_tics.txt");
    ifstream input_suffix("suffix.txt");
    ofstream outputFile(OutputFile);
    string str;

    //input test
    if (!input_prefix.is_open()) 
    {
        cout << "Error: Failed to open the prefix.txt." << endl;
        return;
    }
    else if(!input_set_tics.is_open()) 
    {
        cout << "Error: Failed to open the set_tics.txt." << endl;
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
        center_x = (macro[i][1] + macro[i][3]) / 2;
        center_y = (macro[i][2] + macro[i][4]) / 2;
        cout << "set object " << macro[i][0] << " rect from " << macro[i][1] << "," << macro[i][2] << " to " << macro[i][3] << "," << macro[i][4] << endl;
        cout << "set label \"M" << macro[i][0] << "\" at " << center_x << "," << center_y << " center" << endl; 
        outputFile << "set object " << macro[i][0] << " rect from " << macro[i][1] << "," << macro[i][2] << " to " << macro[i][3] << "," << macro[i][4] << endl;
        outputFile << "set label \"M" << macro[i][0] << "\" at " << center_x << "," << center_y << " center" << endl; 
    }
    
    //write set_tics
    while (getline(input_set_tics, str))
    {
        outputFile << str << endl;
    }

    //write plot [0:boundary_][0:boundary_h]0
    outputFile << "plot" << " " << "[0:" << boundary_w << "]" << "[0:" << boundary_h << "]" << "0" << endl;

    //write suffix
    while (getline(input_suffix, str))
    {
        outputFile << str << endl;
    }
};

