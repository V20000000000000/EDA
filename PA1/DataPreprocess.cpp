#include "DataPreprocess.h"

using namespace std;

DataPreprocess :: DataPreprocess()
{
};

DataPreprocess :: DataPreprocess(string inputFile)
{
    fileName = inputFile;
};

void DataPreprocess :: parseData(vector<vector<int>> &macro,int &boundary_w, int &boundary_h,int &macro_num)
{
    ifstream input(fileName);
    stringstream ss;
    stringstream slabel;
    string str;
    string firstPart;
    string boundary;
    string m_blank;
    int x1, y1, x2, y2;
    int Mlabel;

    //checl input
    if (!input.is_open()) {
        cout << "Error: Failed to open the file." << endl;
        return;
    }

    //find boundary_w, boundary_h, macro_num, Mx coordinate
    while (getline(input, str)) 
    {
        ss.str("");
        ss.clear();
        slabel.str("");
        slabel.clear();

        ss << str;
        slabel << str[1];
        ss >> firstPart;
        slabel >> Mlabel;

        if(firstPart == "Boundary")
        {
            ss >> boundary_w >> boundary_h;
            cout << boundary_w << " " << boundary_h << endl;
        }
        else if(firstPart == "Macro")
        {
            ss >> macro_num;
            cout << macro_num << endl;
        }
        else if(str[0] == 'M')
        {
            ss >> x1 >> y1 >> x2 >> y2;
            macro.push_back({Mlabel, x1, y1, x2, y2});
        } 
    }
};

void DataPreprocess :: display(vector<vector<int>> &macro, int &macro_num)
{
    for(int i = 0; i < macro_num; i++)
    {
        cout << "M" << macro[i][0] << ": " << macro[i][1] << " " << macro[i][2] << " " << macro[i][3] << " " << macro[i][4] << endl;
    }
}