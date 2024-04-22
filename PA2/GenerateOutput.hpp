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

            float chipWidth, chipHeight;
            int Htarget = 0;
            int Vtarget = 0;
            set<int> sourceSetH = graphH->getSourceSet();
            set<int> sourceSetV = graphV->getSourceSet();
            set<int> TargetSetH = graphH->getTargetSet();
            set<int> TargetSetV = graphV->getTargetSet();
            
            std::tie(chipWidth, std::ignore, Htarget) = graphH->findMaxDistance(sourceSetH, TargetSetH);
            std::tie(chipHeight, std::ignore, Vtarget) = graphV->findMaxDistance(sourceSetV, TargetSetV);
            cout << "Chip width: " << chipWidth + graphH->getVertexProperty(Htarget).value.getWidth() << endl;
            cout << "Chip height: " << chipHeight + graphV->getVertexProperty(Vtarget).value.getHeight() << endl;

            outputFile <<  chipWidth + graphH->getVertexProperty(Htarget).value.getWidth() << " " << chipHeight + graphV->getVertexProperty(Vtarget).value.getHeight() << endl;

            for (int i = 0; i < graphH->size(); i++) {
                int x0, y0, x1, y1;
                std::tie(x0, std::ignore) = graphH->findVertexMaxDistance(i, sourceSetH);
                std::tie(y0, std::ignore) = graphV->findVertexMaxDistance(i, sourceSetV);
                int width = graphH->getVertexProperty(i).value.getWidth();
                int height = graphV->getVertexProperty(i).value.getHeight();
                cout << "Node" << i << ": " << "x0 = " << x0 << ", y0 = " << y0 <<  " width: " << width << " height: " << height << endl;

                
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

            float chipWidth, chipHeight;
            int Htarget = 0;
            int Vtarget = 0;
            set<int> sourceSetH = graphH->getSourceSet();
            set<int> sourceSetV = graphV->getSourceSet();
            set<int> TargetSetH = graphH->getTargetSet();
            set<int> TargetSetV = graphV->getTargetSet();
            
            cout << "------------------------------------" << endl;
            std::tie(chipWidth, std::ignore, Htarget) = graphH->findMaxDistance(sourceSetH, TargetSetH);
            std::tie(chipHeight, std::ignore, Vtarget) = graphV->findMaxDistance(sourceSetV, TargetSetV);
            outputFile << "Boundary " << chipWidth + graphH->getVertexProperty(Htarget).value.getWidth() << " " << chipHeight + graphV->getVertexProperty(Vtarget).value.getHeight() << endl;

            outputFile << "Macros " << graphH->size() << endl;

            for (int i = 0; i < graphH->size(); i++) {
                int x0, y0, x1, y1;
                std::tie(x0, std::ignore) = graphH->findVertexMaxDistance(i, sourceSetH);
                std::tie(y0, std::ignore) = graphV->findVertexMaxDistance(i, sourceSetV);
                int width = graphH->getVertexProperty(i).value.getWidth();
                int height = graphV->getVertexProperty(i).value.getHeight();
                cout << "Node" << i << ": " << "x0 = " << x0 << ", y0 = " << y0 <<  " width: " << width << " height: " << height << endl;

                x1 = x0 + width;
                y1 = y0 + height;
                outputFile << "Block_" + to_string(i) + " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;
            }
        }


};


