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
#include "HorizontalGraph.hpp"
#include "InputDataParse.hpp"


using namespace std;

class InitialSolution {
public:
    InitialSolution(string inputFileName)
    {
        // 讀取輸入資料
        InputDataParse parser(inputFileName);
        
        // 創建 HorizontalGraph 物件
        HorizontalGraph<int, int> graph(parser.getNumBlocks());

        // set block width
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            graph.setVertexProperty(i, parser.getBlockWidth(i));
        }
    }
};

std::vector<std::map<std::string, std::vector<int>>> randomPermutation(std::vector<std::map<std::string, std::vector<int>>> blocksList) {
    // 使用C++的隨機數引擎和shuffle函式進行隨機排列
    // 使用當前時間作為隨機種子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    // 隨機排列 blocksList 中的元素
    std::shuffle(blocksList.begin(), blocksList.end(), gen);

    return blocksList;
}

#endif