#ifndef GENERATE_OUTPUT_HPP
#define GENERATE_OUTPUT_HPP
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include "HVGraph.hpp" // 假设这是 HVGraph 类的头文件
#include "Block.hpp"   // 假设这是 Block 类的头文件

using namespace std;

// GenerateOutput 继承自 HVGraph<Block*, int>
class GenerateOutput
{
private:
public:
    GenerateOutput()
    {
        cout << "GenerateOutput constructor" << endl;
    }

    ~GenerateOutput()
    {
        cout << "GenerateOutput destructor" << endl;
    }

    // generate output.txt
    void generateOutputFile(string &outputFileName, vector<string> blockNameList, vector<int> coordinateX, vector<int> coordinateY, int distH, int distV, vector<int> blockWidth, vector <int> blockHeight)
    {
        ofstream outputFile(outputFileName);
        if (!outputFile.is_open())
        {
            cout << "Error: Unable to open output file." << endl;
            return;
        }

        int chipWidth, chipHeight;

        chipWidth = distH;
        chipHeight = distV;
        cout << "Chip width: " << distH << endl;
        cout << "Chip height: " << distV << endl;

        outputFile << chipWidth << " " << chipHeight << endl;

        for (unsigned int i = 0; i < coordinateX.size(); i++)
        {
            int x0, y0, x1, y1;
            x0 = coordinateX[i];
            y0 = coordinateY[i];
            int width = /*blockWidth[i]*/ 1;
            int height = /*blockHeight[i]*/ 1;
            x1 = x0 + width;
            y1 = y0 + height;
            outputFile << blockNameList[i] << " ";
            outputFile << width << " " << height << " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
        }

        outputFile.close();
    }
};

#endif
