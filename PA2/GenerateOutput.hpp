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
        HVGraph<Block, int> graphH;
        HVGraph<Block, int> graphV;
    public:
        GenerateOutput(HVGraph<Block, int> &graph1, HVGraph<Block, int> &graph2) : graphH(graph1) , graphV(graph2)
        {
            float chipWidth, chipHeight;
            std::tie(chipWidth, std::ignore, std::ignore) = graphH.findMaxDistance(0);
            std::tie(chipHeight, std::ignore, std::ignore) = graphV.findMaxDistance(1);
        }

        // generate output.txt
        void generateOutputFile(string &outputFileName) 
        {
            ofstream outputFile(outputFileName);
            if (!outputFile.is_open()) {
                cout << "Error: Unable to open output file." << endl;
                return;
            }

            float chipWidth, chipHeight;
            std::tie(chipWidth, std::ignore, std::ignore) = graphH.findMaxDistance(0);
            std::tie(chipHeight, std::ignore, std::ignore) = graphV.findMaxDistance(1);
            outputFile << chipWidth << " " << chipHeight << endl;

            for (int i = 0; i < graphH.size(); i++) {
                int x0, y0, x1, y1;
                std::tie(x1, std::ignore) = graphH.findVertexMaxDistance(i, 0);
                std::tie(y1, std::ignore) = graphV.findVertexMaxDistance(i, 0);
                int width = graphH.getVertexProperty(i).value.getWidth();
                int height = graphV.getVertexProperty(i).value.getHeight();
                x0 = x1 - width;
                y0 = y1 - height;
                outputFile << "Block_" + to_string(i) + " "  << width << " " << height << " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
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

            float chipWidth, chipHeight;
            std::tie(chipWidth, std::ignore, std::ignore) = graphH.findMaxDistance(0);
            std::tie(chipHeight, std::ignore, std::ignore) = graphV.findMaxDistance(1);
            outputFile << "Boundary " << chipWidth << " " << chipHeight << endl;

            outputFile << "Macros " << graphH.size() << endl;

            for (int i = 0; i < graphH.size(); i++) {
                int x0, y0, x1, y1;
                std::tie(x1, std::ignore) = graphH.findVertexMaxDistance(i, 0);
                std::tie(y1, std::ignore) = graphV.findVertexMaxDistance(i, 0);
                int width = graphH.getVertexProperty(i).value.getWidth();
                int height = graphV.getVertexProperty(i).value.getHeight();
                x0 = x1 - width;
                y0 = y1 - height;
                outputFile << "Block_" + to_string(i) + " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
            }
        }


};


