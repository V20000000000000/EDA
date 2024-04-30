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
    HVGraph<Block, int> graphH;
    HVGraph<Block, int> graphV;

public:
    GenerateOutput(HVGraph<Block, int> graph1, HVGraph<Block, int> graph2) : graphH(graph1), graphV(graph2)
    {
        // graphH -> rotateBlock(6);
        // graphV -> rotateBlock(6);

        // graphH -> recalculateVertexEdgeWeight(6, 0);
        // graphV -> recalculateVertexEdgeWeight(6, 1);
        cout << "GenerateOutput constructor" << endl;
        cout << "getWidth" << graphH.getVertexProperty(5).value.getWidth() << endl;
        cout << "source" << endl;
        cout << graph1.getEdgeWeight(5, 11) << endl;
        cout << graph1.getEdgeWeight(10, 5) << endl;
        cout << graph1.size();
        cout << endl;
    }   


    // generate output.txt
    void generateOutputFile(string &outputFileName, vector<string> blockNameList)
    {
        cout << "test" << endl;
        ofstream outputFile(outputFileName);
        if (!outputFile.is_open())
        {
            cout << "Error: Unable to open output file." << endl;
            return;
        }

        int chipWidth, chipHeight;

        chipWidth = graphH.calculateMaxTotalEdgeWeight(graphH.size(), graphH.size() + 1);
        chipHeight = graphV.calculateMaxTotalEdgeWeight(graphV.size(), graphV.size() + 1);
        cout << "Chip width: " << chipWidth << endl;
        cout << "Chip height: " << chipHeight << endl;

        outputFile << chipWidth << " " << chipHeight << endl;

        for (int i = 0; i < graphH.size(); i++)
        {
            int x0, y0, x1, y1;
            x0 = graphH.calculateMaxTotalEdgeWeight(graphH.size(), i);
            y0 = graphV.calculateMaxTotalEdgeWeight(graphV.size(), i);
            int width = graphH.getVertexProperty(i).value.getWidth();
            int height = graphV.getVertexProperty(i).value.getHeight();

            cout << "x0: " << x0 << " " << "y0" << y0 << endl;

            x1 = x0 + width;
            y1 = y0 + height;
            outputFile << blockNameList[i] << " ";
            outputFile << width << " " << height << " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
        }
    }
};

#endif
