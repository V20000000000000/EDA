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
#include "Timer.hpp"


using namespace std;

void randomPermutation(vector<int>& Permutation);

class InitialSolution {
public:
    InitialSolution(string inputFileName)
    {
        // 讀取輸入資料
        InputDataParse parser(inputFileName);
        
        // 創建 HorizontalGraph 物件
        HorizontalGraph<int, int> graph(parser.getNumBlocks());

        // set block width in the graph
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            graph.setVertexProperty(i, parser.getBlockWidth(i));
        }

        // 初始化 block水平順序和垂直順序
        vector<int> horizontalPermutation(parser.getNumBlocks());
        vector<int> verticalPermutation(parser.getNumBlocks());
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            horizontalPermutation[i] = i;
            verticalPermutation[i] = i;
        }

        Timer timerRandomPermutation;
        timerRandomPermutation.start();
        // 隨機排列 block水平順序和垂直順序
        randomPermutation(horizontalPermutation);
        randomPermutation(verticalPermutation);
        int t = timerRandomPermutation.stop();


        cout << "Time taken to generate random permutation: " << t << " ms" << endl;

        //display the horizontal permutation
        cout << "Horizontal Permutation: ";
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            cout << horizontalPermutation[i] << " ";
        }
        cout << endl;

        //display the vertical permutation
        cout << "Vertical Permutation: ";
        for (int i = 0; i < parser.getNumBlocks(); ++i) {
            cout << verticalPermutation[i] << " ";
        }
        cout << endl;
    }
};

void randomPermutation(vector<int>& Permutation) {
    // 使用C++的隨機數引擎和shuffle函式進行隨機排列
    // 使用當前時間作為隨機種子
    // 隨機排列 block水平順序和垂直順序
    unsigned Seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 Gen(Seed);
    cout << "seed: " << Seed << endl;
    std::shuffle(Permutation.begin(), Permutation.end(), Gen);
}

#endif