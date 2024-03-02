#ifndef DataPreprocess_H
#define DataPreprocess_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class DataPreprocess
{
    private:
        string fileName;
    public:
        DataPreprocess();
        DataPreprocess(string inputFile);
        void parseData(vector<vector<int>> &macro, vector<string> &macro_label, int &boundary_w, int &boundary_h,int &macro_num);
        void display(vector<vector<int>> &macro, vector<string> &macro_label, int &macro_num);
};

#endif