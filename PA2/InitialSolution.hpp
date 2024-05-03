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
pair<int, int> checkRatio(float minAspectRatio, float maxAspectRatio, int width, int height, int id);

class InitialSolution
{
private:
    HVGraph<Block *, int> *horizontalGraph;
    HVGraph<Block *, int> *verticalGraph;
    InputDataParse *parser;

public:
    ~InitialSolution()
    {
        delete horizontalGraph;
        delete verticalGraph;
        delete parser;
    }

    InitialSolution(string inputFileName)
    {
        parser = new InputDataParse(inputFileName);
        horizontalGraph = new HVGraph<Block *, int>(parser->getNumBlocks() + 2);
        verticalGraph = new HVGraph<Block *, int>(parser->getNumBlocks() + 2);

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
        //randomPermutation(horizontalPermutation);
        // 隨機排列 block垂直順序(向量中的值是index block的垂直y位置)
        //randomPermutation(verticalPermutation);

        // 把每個block的資料放進去(HorizontalGraph)(VerticalGraph)
        float minAspectRatio = parser->getMinAspectRatio();
        float maxAspectRatio = parser->getMaxAspectRatio();
        cout << "minAspectRatio: " << minAspectRatio << " maxAspectRatio: " << maxAspectRatio << endl;
        for (int i = 0; i < parser->getNumBlocks(); ++i)
        {
            pair<int, int> widthHeight = checkRatio(minAspectRatio, maxAspectRatio, parser->getBlockWidth(i), parser->getBlockHeight(i), i);
            //cout << "Block_" << i << " width: " << widthHeight.first << " height: " << widthHeight.second << endl;
            Block *horizontalBlock = new Block(i, widthHeight.first, widthHeight.second); // Dynamically allocate memory for a new block for HorizontalGraph
            Block *verticalBlock = new Block(i, widthHeight.first, widthHeight.second);   // Dynamically allocate memory for a new block for VerticalGraph

            horizontalBlock->setX(horizontalPermutation[i]);
            horizontalBlock->setY(verticalPermutation[i]);
            horizontalBlock->setWeight((horizontalPermutation[i] + verticalPermutation[i]));
            horizontalGraph->setVertexProperty(i, horizontalBlock);

            verticalBlock->setX(horizontalPermutation[i]);
            verticalBlock->setY(verticalPermutation[i]);
            verticalBlock->setWeight((verticalPermutation[i] - horizontalPermutation[i]));
            verticalGraph->setVertexProperty(i, verticalBlock);

            // horizontalBlocksPointers[i] = horizontalBlock; // Store the pointer to the dynamically allocated block for HorizontalGraph
            // verticalBlocksPointers[i] = verticalBlock;     // Store the pointer to the dynamically allocated block for VerticalGraph
        }

        // 把source跟target放進去(HorizontalGraph)(VerticalGraph)
        Block *sourceBlockH = new Block(parser->getNumBlocks(), 0, 0);     // Dynamically allocate memory for a new block for HorizontalGraph
        Block *targetBlockH = new Block(parser->getNumBlocks() + 1, 0, 0); // Dynamically allocate memory for a new block for HorizontalGraph
        // horizontalBlocksPointers[parser->getNumBlocks()] = sourceBlockH;
        // horizontalBlocksPointers[parser->getNumBlocks() + 1] = targetBlockH;
        horizontalGraph->setVertexProperty(parser->getNumBlocks(), sourceBlockH);
        horizontalGraph->setVertexProperty(parser->getNumBlocks() + 1, targetBlockH);

        Block *sourceBlockV = new Block(parser->getNumBlocks(), 0, 0);     // Dynamically allocate memory for a new block for VerticalGraph
        Block *targetBlockV = new Block(parser->getNumBlocks() + 1, 0, 0); // Dynamically allocate memory for a new block for VerticalGraph
        // verticalBlocksPointers[parser->getNumBlocks()] = sourceBlockV;
        // verticalBlocksPointers[parser->getNumBlocks() + 1] = targetBlockV;
        verticalGraph->setVertexProperty(parser->getNumBlocks(), sourceBlockV);
        verticalGraph->setVertexProperty(parser->getNumBlocks() + 1, targetBlockV);

        cout << "build edges between nodes:" << endl;
        // 建立點跟點之間的Edges(HorizontalGraph)
        // 建立directedEdge的條件: (source的x座標 < target的x座標) && (source的y座標 < target的y座標)
        int sWeight = 0;
        for (int t = 0; t < parser->getNumBlocks(); t++)
        {
            for (int s = 0; s < parser->getNumBlocks(); ++s)
            {
                if (s != t)
                {
                    int x1 = horizontalGraph->getVertexProperty(s).value->getX();
                    int y1 = horizontalGraph->getVertexProperty(s).value->getY();
                    int x2 = horizontalGraph->getVertexProperty(t).value->getX();
                    int y2 = horizontalGraph->getVertexProperty(t).value->getY();

                    if (x1 < x2 && y1 < y2)
                    {
                        horizontalGraph->addDirectedEdge(s, t, horizontalGraph->getVertexProperty(s).value->getWidth());
                    }
                }
            }
            // source to t 
            horizontalGraph->addDirectedEdge(parser->getNumBlocks(), t, 0);
            // t to target
            horizontalGraph->addDirectedEdge(t, parser->getNumBlocks() + 1, horizontalGraph->getVertexProperty(t).value->getWidth());
        }

        // 建立點跟點之間的Edges(VerticalGraph)
        // 建立directedEdge的條件: (source的x座標 > target的x座標) && (source的y座標 < target的y座標) && (source weight < target weight)
        for (int s = 0; s < parser->getNumBlocks(); s++)
        {
            for (int t = 0; t < parser->getNumBlocks(); ++t)
            {
                if (t != s)
                {
                    int x1 = verticalGraph->getVertexProperty(s).value->getX();
                    int y1 = verticalGraph->getVertexProperty(s).value->getY();
                    int x2 = verticalGraph->getVertexProperty(t).value->getX();
                    int y2 = verticalGraph->getVertexProperty(t).value->getY();

                    if (x1 > x2 && y1 < y2)
                    {
                        verticalGraph->addDirectedEdge(s, t, verticalGraph->getVertexProperty(s).value->getHeight());
                    }
                }
            }
            // source to s
            verticalGraph->addDirectedEdge(parser->getNumBlocks(), s, 0);
            // s to target
            verticalGraph->addDirectedEdge(s, parser->getNumBlocks() + 1, verticalGraph->getVertexProperty(s).value->getHeight());
        }


        cout << "findMaxDistance:" << endl;
        // 找到從起點開始的最大距離(HorizontalGraph)
        cout << "HorizontalGraph" << endl;
        float MaxDiatance;
        int source;
        int target;

        // 輸出每個vertex的最大距離(VerticalGraph)
        for (int i = 0; i < parser->getNumBlocks(); i++)
        {
            // std::tie(MaxDiatance, source) = VerticalGraph->findVertexMaxDistance(i, sourceSetV);
            // cout << "Block_" << i << " Max Distance: " << MaxDiatance << " from " << source << " (first node)" << endl;
        }

        horizontalGraph = horizontalGraph;
        verticalGraph = verticalGraph;
        cout << "----------------------------------" << endl;
    }

    HVGraph<Block *, int> *getHorizontalGraph() const
    {
        return horizontalGraph;
    }

    HVGraph<Block *, int> *getVerticalGraph() const
    {
        return verticalGraph;
    }

    vector<string> getBlockNameList() const
    {
        return parser->getBlocksNameList();
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

pair<int, int> checkRatio(float minAspectRatio, float maxAspectRatio, int width, int height, int id)
{
    float ratio = static_cast<float>(width) / static_cast<float>(height);
    if ((ratio < minAspectRatio) || (ratio > maxAspectRatio))
    {
        int area = width * height;
        if (ratio < minAspectRatio)
        {
            for (int i = width + 1; i < height; i++) // Increment i by 1 and fix loop condition
            {
                if (area % i == 0)
                {
                    int j = area / i;
                    return checkRatio(minAspectRatio, maxAspectRatio, i, j, id);
                }
            }
        }
        else if (ratio > maxAspectRatio)
        {
            for (int i = height + 1; i < width; i++) // Increment i by 1 and fix loop condition
            {
                if (area % i == 0)
                {
                    int j = area / i;
                    return checkRatio(minAspectRatio, maxAspectRatio, i, j, id);
                }
            }
        }
    }
    return make_pair(width, height);
}

#endif