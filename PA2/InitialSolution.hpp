#ifndef INITIAL_SOLUTION_HPP
#define INITIAL_SOLUTION_HPP

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <algorithm>
#include <chrono>
#include <tuple>
#include "HVGraph.hpp"
#include "InputDataParse.hpp"
#include "Timer.hpp"
#include "Block.hpp"

using namespace std;

void randomPermutation(vector<int> &Permutation);

class InitialSolution
{
private:
    HVGraph<Block, int> *HorizontalGraph;
    HVGraph<Block, int> *VerticalGraph;
    InputDataParse *parser;

public:
    ~InitialSolution()
    {
        delete HorizontalGraph;
        delete VerticalGraph;
        delete parser;
    }

    InitialSolution(string inputFileName)
    {
        parser = new InputDataParse(inputFileName);
        HorizontalGraph = new HVGraph<Block, int>(parser->getNumBlocks() + 2);
        VerticalGraph = new HVGraph<Block, int>(parser->getNumBlocks() + 2);

        cout << "start initialSolution" << endl;
        // 初始化 block水平垂直順序
        vector<int> horizontalPermutation(parser->getNumBlocks());
        vector<int> verticalPermutation(parser->getNumBlocks());
        for (int i = 0; i < parser->getNumBlocks(); ++i)
        {
            horizontalPermutation[i] = i;
            verticalPermutation[i] = i;
        }
        cout << "parser.getNumBlocks(): " << parser->getNumBlocks() << endl;
        // 隨機排列 block水平順序(向量中的值是index block的水平x位置)
        randomPermutation(horizontalPermutation);
        // 隨機排列 block垂直順序(向量中的值是index block的垂直y位置)
        randomPermutation(verticalPermutation);
        //horizontalPermutation = {1, 2, 5, 3, 4, 6};
        //verticalPermutation = {4, 2, 1, 5, 6, 3};

        // Container to store pointers to blocks
        vector<Block *> horizontalBlocksPointers(parser->getNumBlocks() + 2);
        vector<Block *> verticalBlocksPointers(parser->getNumBlocks() + 2);

        // 把每個block的資料放進去(HorizontalGraph)(VerticalGraph)
        for (int i = 0; i < parser->getNumBlocks(); ++i)
        {
            Block *horizontalBlock = new Block(i, parser->getBlockWidth(i), parser->getBlockHeight(i)); // Dynamically allocate memory for a new block for HorizontalGraph
            Block *verticalBlock = new Block(i, parser->getBlockWidth(i), parser->getBlockHeight(i));   // Dynamically allocate memory for a new block for VerticalGraph
            horizontalBlock->setWidth(parser->getBlockWidth(i));
            // cout << "block_" << i << " width: " << parser.getBlockWidth(i) << endl;
            horizontalBlock->setHeight(parser->getBlockHeight(i));
            horizontalBlock->setX(horizontalPermutation[i]);
            horizontalBlock->setY(verticalPermutation[i]);
            horizontalBlock->setWeight((horizontalPermutation[i] + verticalPermutation[i]));
            HorizontalGraph->setVertexProperty(i, *horizontalBlock);

            verticalBlock->setWidth(parser->getBlockWidth(i));
            verticalBlock->setHeight(parser->getBlockHeight(i));
            verticalBlock->setX(horizontalPermutation[i]);
            verticalBlock->setY(verticalPermutation[i]);
            verticalBlock->setWeight((verticalPermutation[i] - horizontalPermutation[i]));
            VerticalGraph->setVertexProperty(i, *verticalBlock);

            horizontalBlocksPointers[i] = horizontalBlock; // Store the pointer to the dynamically allocated block for HorizontalGraph
            verticalBlocksPointers[i] = verticalBlock;     // Store the pointer to the dynamically allocated block for VerticalGraph
        }

        // 把source跟target放進去(HorizontalGraph)(VerticalGraph)
        Block *sourceBlockH = new Block(parser->getNumBlocks(), 0, 0); // Dynamically allocate memory for a new block for HorizontalGraph
        Block *targetBlockH = new Block(parser->getNumBlocks() + 1, 0, 0); // Dynamically allocate memory for a new block for HorizontalGraph
        horizontalBlocksPointers[parser->getNumBlocks()] = sourceBlockH;
        horizontalBlocksPointers[parser->getNumBlocks() + 1] = targetBlockH;
        HorizontalGraph->setVertexProperty(parser->getNumBlocks(), *sourceBlockH);

        Block *sourceBlockV = new Block(parser->getNumBlocks(), 0, 0); // Dynamically allocate memory for a new block for VerticalGraph
        Block *targetBlockV = new Block(parser->getNumBlocks() + 1, 0, 0); // Dynamically allocate memory for a new block for VerticalGraph
        verticalBlocksPointers[parser->getNumBlocks()] = sourceBlockV;
        verticalBlocksPointers[parser->getNumBlocks() + 1] = targetBlockV;
        VerticalGraph->setVertexProperty(parser->getNumBlocks(), *sourceBlockV);

        cout << "build edges between nodes:" << endl;
        // 建立點跟點之間的Edges(HorizontalGraph)
        // 建立directedEdge的條件: (source的x座標 < target的x座標) && (source的y座標 < target的y座標) && (source weight < target weight)
        int sWeight = 0;
        for (int t = 0; t < parser->getNumBlocks(); t++)
        {
            for (int s = 0; s < parser->getNumBlocks(); ++s)
            {
                if (s != t)
                {
                    if (HorizontalGraph->getVertexProperty(s).value.getWeight() < HorizontalGraph->getVertexProperty(t).value.getWeight())
                    {
                        if ((HorizontalGraph->getVertexProperty(s).value.getX() < HorizontalGraph->getVertexProperty(t).value.getX()) && (HorizontalGraph->getVertexProperty(s).value.getY() < HorizontalGraph->getVertexProperty(t).value.getY()))
                        {
                            // cout << "s.x: " << Horizontalgraph.getVertexProperty(s).value.getX() << " s.y: " << Horizontalgraph.getVertexProperty(s).value.getY() << " t.x: " << Horizontalgraph.getVertexProperty(t).value.getX() << " t.y: " << Horizontalgraph.getVertexProperty(t).value.getY() << endl;
                            HorizontalGraph->addDirectedEdge(s, t, HorizontalGraph->getVertexProperty(s).value.getWidth());
                        }
                    }
                }
            }
        }

        // 建立點跟點之間的Edges(VerticalGraph)
        // 建立directedEdge的條件: (source的x座標 > target的x座標) && (source的y座標 < target的y座標) && (source weight < target weight)
        for (int s = 0; s < parser->getNumBlocks(); s++)
        {
            for (int t = 0; t < parser->getNumBlocks(); ++t)
            {
                if (t != s)
                {
                    if (VerticalGraph->getVertexProperty(s).value.getWeight() < VerticalGraph->getVertexProperty(t).value.getWeight())
                    {
                        if ((VerticalGraph->getVertexProperty(s).value.getX() > VerticalGraph->getVertexProperty(t).value.getX()) && (VerticalGraph->getVertexProperty(s).value.getY() < VerticalGraph->getVertexProperty(t).value.getY()))
                        {
                            // cout << "s.x: " << Verticalgraph.getVertexProperty(s).value.getX() << " s.y: " << Verticalgraph.getVertexProperty(s).value.getY() << " t.x: " << Verticalgraph.getVertexProperty(t).value.getX() << " t.y: " << Verticalgraph.getVertexProperty(t).value.getY() << endl;
                            VerticalGraph->addDirectedEdge(s, t, VerticalGraph->getVertexProperty(s).value.getHeight());
                        }
                    }
                }
            }
        }

        // 建立source跟最左邊那幾個點的Edges(HorizontalGraph) 
        for (int i = 0; i < parser->getNumBlocks(); i++)
        {
            HorizontalGraph->addDirectedEdge(parser->getNumBlocks(), i, 0);
            VerticalGraph->addDirectedEdge(parser->getNumBlocks(), i, 0);

            HorizontalGraph->addDirectedEdge(i, parser->getNumBlocks() + 1, HorizontalGraph->getVertexProperty(i).value.getWidth());
            VerticalGraph->addDirectedEdge(i, parser->getNumBlocks() + 1, VerticalGraph->getVertexProperty(i).value.getHeight());
        }

        cout << "findMaxDistance:" << endl;
        // 找到從起點開始的最大距離(HorizontalGraph)
        cout << "HorizontalGraph" << endl;
        float MaxDiatance;
        int source;
        int target;
        // std::tie(MaxDiatance, source, target) = HorizontalGraph->findMaxDistance(sourceSetH, targetSetH);
        // cout << "Max Distance: " << MaxDiatance << " Node next to Source : " << source << " Target: " << target << endl;

        // 輸出每個vertex的最大距離(HorizontalGraph)
        // for (int i = 0; i < parser->getNumBlocks(); i++)
        // {
        //     std::tie(MaxDiatance, source) = HorizontalGraph->findVertexMaxDistance(i, sourceSetH);
        //     //cout << "Block_" << i << " Max Distance: " << MaxDiatance << " from " << source << " (first node)" << endl;
        // }

        // 找到從起點開始的最大距離(VerticalGraph)
        //cout << "VerticalGraph" << endl;
        // std::tie(MaxDiatance, source, target) = VerticalGraph->findMaxDistance(sourceSetV, targetSetV);
        //cout << "Max Distance: " << MaxDiatance << " Node next to Source : " << source << " Target: " << target << endl;

        // 輸出每個vertex的最大距離(VerticalGraph)
        for (int i = 0; i < parser->getNumBlocks(); i++)
        {
            //std::tie(MaxDiatance, source) = VerticalGraph->findVertexMaxDistance(i, sourceSetV);
            //cout << "Block_" << i << " Max Distance: " << MaxDiatance << " from " << source << " (first node)" << endl;
        }

        // 將Horizontalgraph Verticalgraph的資料存入HorizontalGraph VerticalGraph
        HorizontalGraph = HorizontalGraph;
        VerticalGraph = VerticalGraph;
        cout << "----------------------------------" << endl;
    }

    HVGraph<Block, int> *getHorizontalGraph() const
    {
        return HorizontalGraph;
    }

    HVGraph<Block, int> *getVerticalGraph() const
    {
        return VerticalGraph;
    }
};

void randomPermutation(vector<int> &Permutation)
{
    // 使用C++的隨機數引擎和shuffle函式進行隨機排列
    // 使用提供的隨機數生成器
    static std::random_device rd;
    unsigned int seed1, seed2;
    seed1 = rd();
    seed2 = chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed1 + seed2);
    // 使用 mt19937 引擎生成隨機數
    std::shuffle(Permutation.begin(), Permutation.end(), gen);
}

#endif