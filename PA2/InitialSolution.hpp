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
#include "HVGraph.hpp"
#include "InputDataParse.hpp"
#include "Timer.hpp"
#include "Block.hpp"

using namespace std;

void randomPermutation(vector<int>& Permutation);

class InitialSolution {
public:
    InitialSolution(string inputFileName) {
        // 讀取輸入資料
        InputDataParse parser(inputFileName);
        
        // 創建 HorizontalGraph 物件
        HVGraph<Block, int> Horizontalgraph (parser.getNumBlocks());
        HVGraph<Block, int> Verticalgraph (parser.getNumBlocks());

        // 初始化 block水平垂直順序
        vector<int> horizontalPermutation(parser.getNumBlocks());
        vector<int> verticalPermutation(parser.getNumBlocks());
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            horizontalPermutation[i] = i;
            verticalPermutation[i] = i;
        }

        // 隨機排列 block水平順序(向量中的值是index block的水平x位置)
        randomPermutation(horizontalPermutation);
        // 隨機排列 block垂直順序(向量中的值是index block的垂直y位置)
        randomPermutation(verticalPermutation);
        
        // 把每個block的資料放進去(HorizontalGraph)(VerticalGraph)
        for(int i = 0; i < parser.getNumBlocks(); ++i) {
            Block a(i, parser.getBlockWidth(i), parser.getBlockHeight(i));
            a.setWidth(parser.getBlockWidth(i));
            a.setHeight(parser.getBlockHeight(i));
            a.setX(horizontalPermutation[i]);
            a.setY(verticalPermutation[i]);
            a.setWeight((horizontalPermutation[i] + verticalPermutation[i]));
            Horizontalgraph.setVertexProperty(i, a); 
            a.setWeight((verticalPermutation[i] - horizontalPermutation[i]));
            Verticalgraph.setVertexProperty(i, a);
        }

        // 輸出每個vertex中的x y值(HorizontalGraph)
        cout << "VerticalGraph" << endl;
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            //cout << "block_" << i << " x: " << Horizontalgraph.getVertexProperty(i).value.getX() << " y: " << Horizontalgraph.getVertexProperty(i).value.getY() << endl;
        }

        // 輸出每個vertex中的x y值(VerticalGraph)
        cout << "VerticalGraph" << endl;
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            //cout << "block_" << i << " x: " << Verticalgraph.getVertexProperty(i).value.getX() << " y: " << Verticalgraph.getVertexProperty(i).value.getY() << endl;
        }
    }
};

void randomPermutation(vector<int>& Permutation) {
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
