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

// GenerateOutput 继承自 HVGraph<Block, int>
class GenerateOutput
{
    private:
        HVGraph<Block, int> *graphH;
        HVGraph<Block, int> *graphV;
    public:
        GenerateOutput(HVGraph<Block, int> *graph1, HVGraph<Block, int> *graph2) : graphH(graph1) , graphV(graph2){}

        // generate output.txt
        void generateOutputFile(string &outputFileName) 
        {
            ofstream outputFile(outputFileName);
            if (!outputFile.is_open()) {
                cout << "Error: Unable to open output file." << endl;
                return;
            }

            int chipWidth, chipHeight;
            
            chipWidth = graphH->calculateMaxTotalEdgeWeight(graphH->size(), graphH->size() + 1);
            chipHeight = graphV->calculateMaxTotalEdgeWeight(graphV->size(), graphV->size() + 1);
            cout << "Chip width: " << chipWidth << endl;
            cout << "Chip height: " << chipHeight << endl;

            outputFile << chipWidth << " " << chipHeight << endl;

            for (int i = 0; i < graphH->size(); i++) {
                int x0, y0, x1, y1;
                x0 = graphH->calculateMaxTotalEdgeWeight(graphH->size(), i);
                y0 = graphV->calculateMaxTotalEdgeWeight(graphV->size(), i);
                int width = graphH->getVertexProperty(i).value.getWidth();
                int height = graphV->getVertexProperty(i).value.getHeight();
                //cout << "Node" << i << ": " << "x0 = " << x0 << ", y0 = " << y0 <<  " width: " << width << " height: " << height << endl;
                
                x1 = x0 + width;
                y1 = y0 + height;
                outputFile << "Block_" + to_string(i) << " ";
                outputFile << width << " " << height << " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;;
            }
        }

        // generate input.txt for PA1 transfer
        void generatePA1inputFile() 
        {
            ofstream outputFile("PA1input.txt");
            if (!outputFile.is_open()) {
                cout << "Error: Unable to open input file." << endl;
                return;
            }

            int chipWidth, chipHeight;
            
            cout << "------------------------------------" << endl;
            chipWidth = graphH->calculateMaxTotalEdgeWeight(graphH->size(), graphH->size() + 1);
            chipHeight = graphV->calculateMaxTotalEdgeWeight(graphV->size(), graphV->size() + 1);
            outputFile << "Boundary " << chipWidth << " " << chipHeight << endl;

            outputFile << "Macros " << graphH->size() << endl;

            for (int i = 0; i < graphH->size(); i++) {
                int x0, y0, x1, y1;
                x0 = graphH->calculateMaxTotalEdgeWeight(graphH->size(), i);
                y0 = graphV->calculateMaxTotalEdgeWeight(graphV->size(), i);
                int width = graphH->getVertexProperty(i).value.getWidth();
                int height = graphV->getVertexProperty(i).value.getHeight();
                cout << "Node" << i << ": " << "x0 = " << x0 << ", y0 = " << y0 <<  " width: " << width << " height: " << height << endl;

                x1 = x0 + width;
                y1 = y0 + height;
                outputFile << "Block_" + to_string(i) + " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
            }
        }
};

#endif



