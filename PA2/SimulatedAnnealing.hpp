#ifndef SIMULATED_ANNEALING_HPP
#define SIMULATED_ANNEALING_HPP

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <tuple>
#include <algorithm>
#include <chrono>
#include <fstream>

#include "HVgraph.hpp"
#include "Block.hpp"
#include "Timer.hpp"

using namespace std;

int getRandomOperation();
int getRandomBlock(int numBlock);
float getRandomProbability();

class SimulatedAnnealing
{
private:
    double temperature;
    double coolingRate;
    int MaxDistanceH;
    int MaxDistanceV;
    int initialDistanceH;
    int initialDistanceV;
    int GlobalBestH;
    int GlobalBestV;
    int initialCost;
    int bestCost;
    int GlobalBestCost;
    int currentCost;
    int move1count;
    int move2count;
    int move3count;
    int move4count;
    int accpetCount;
    int rejectCount;
    int P;
    int N;
    int preoperation;
    HVGraph<Block *, int> *graphH;
    HVGraph<Block *, int> *graphV;
    vector<int> coordinateX;
    vector<int> coordinateY;

    pair<int, int> preresult;

public:
    // Constructor
    SimulatedAnnealing(HVGraph<Block *, int> *graphH, HVGraph<Block *, int> *graphV) : graphH(graphH), graphV(graphV), coordinateX(graphH->size()), coordinateY(graphH->size()) {}
    // Destructor
    ~SimulatedAnnealing() {}

    void run()
    {
        // Initialize the timer
        Timer timer;
        timer.start();
        // Initialize the temperature
        temperature = 10000;
        int step = 0;

        // Set cooling rate
        coolingRate = 0.85;

        // Initialize the input size
        N = graphH->size();
        // Initialize the current cost
        currentCost = 0;

        // initialize the move count
        move1count = 0;
        move2count = 0;
        move3count = 0;
        move4count = 0;
        accpetCount = 0;
        rejectCount = 0;
        // Set the number of iterations
        P = 7 * N;
        

        // Get the chip width and height
        //cout << "--------------------------" << endl;
        MaxDistanceH = graphH->calculateMaxTotalEdgeWeight(graphH->size(), graphH->size() + 1);
        MaxDistanceV = graphV->calculateMaxTotalEdgeWeight(graphV->size(), graphV->size() + 1);
        // cout << "--------------------------" << endl;
        // cout << "MaxDistanceH: " << MaxDistanceH << " MaxDistanceV: " << MaxDistanceV << endl;
        // cout << "--------------------------" << endl;

        // int preDistH = MaxDistanceH;
        // int preDistV = MaxDistanceV;

        // Initialize the best cost
        initialCost = max(MaxDistanceH, MaxDistanceV);
        initialDistanceH = MaxDistanceH;
        initialDistanceV = MaxDistanceV;
        cout << "Initial DistH: " << initialDistanceH << endl;
        cout << "Initial DistV: " << initialDistanceV << endl;
        bestCost = max(initialDistanceH, initialDistanceV);
        GlobalBestCost = bestCost;

        bool run = true;
        ofstream outputFile("log.txt");
        // Loop until the temperature is zero
        while (temperature > 1 && run)
        {
            for (int i = P; i >= 1; i--)
            {
                if(timer.elapsed() > 500000)
                {
                    run = false;
                }
                step = step + 1;
                //preDistH = MaxDistanceH;
                //preDistV = MaxDistanceV;
                
                // cout << "preDistH: " << preDistH << " ";
                // cout << "preDistV: " << preDistV << endl;

                // random select a operation
                int operation = getRandomOperation();
                //cout << "Operation: " << operation << endl;
                
                // Create a new solution
                // cout << "Operation: " << operation << endl;
                getNewSolution(operation, preresult, false);

                // Calculate the cost of the new solution
                // update the MaxDistanceH and MaxDistanceV
                MaxDistanceH = graphH->calculateMaxTotalEdgeWeight(graphH->size(), graphH->size() + 1);
                MaxDistanceV = graphV->calculateMaxTotalEdgeWeight(graphV->size(), graphV->size() + 1);
                // cout << "MaxDistanceH: " << MaxDistanceH << " MaxDistanceV: " << MaxDistanceV << endl;
                currentCost = max(MaxDistanceH, MaxDistanceV);

                // Calculate the cost difference
                int costDifference = currentCost - bestCost;
                //store globalbest solution
                if(currentCost < GlobalBestCost)
                {
                    GlobalBestCost = currentCost;
                    GlobalBestH = MaxDistanceH;
                    GlobalBestV = MaxDistanceV;
                    storeGlobalBestGraph();
                }

                // If the new solution is better, accept it
                if (currentCost < bestCost)
                {
                    bestCost = currentCost;
                }
                else // If the new solution is worse, accept it with a probability
                {
                    double probabilityThreshold;
                    probabilityThreshold = exp(-costDifference / temperature);

                    double probability = getRandomProbability();

                    if (probability <= probabilityThreshold)
                    {
                        // Accept the new solution
                        bestCost = currentCost;
                        accpetCount++;
                    }
                    else
                    {
                        //cout << "preresult: " << preresult.first << " " << preresult.second << endl;
                        //cout << "operation: " << operation << endl;
                        getNewSolution(operation, preresult, true); // Back to the original solution
                        //cout << "reject" << endl;
                        rejectCount++;
                    }
                }
                // cout << endl;
                // If the new solution is the best, update the best solution
                //cout << "--------------------------" << endl;
            }
            // Cool the temperature
            cout << "step: " << step << " ";
            cout << "Best solution: " << GlobalBestCost << endl;
            outputFile << "step: " << step << " ";
            outputFile << "Best solution: " << GlobalBestCost << endl;
            // cout << "Temperature: " << temperature << endl;
            //cout << "MaxDistanceH: " << MaxDistanceH << " MaxDistanceV: " << MaxDistanceV << endl;
            temperature = temperature * coolingRate;
        }

        // Output the best solution
        cout << "Initial solution: " << initialCost << endl;
        cout << "DistanceH: " << initialDistanceH << " DistanceV: " << initialDistanceV << endl;
        cout << "Best solution: " << GlobalBestCost << endl;
        cout << "DistanceH: " << GlobalBestH << " DistanceV: " << GlobalBestV << endl;
        cout << "Move1 count: " << move1count << endl;
        cout << "Move2 count: " << move2count << endl;
        cout << "Move3 count: " << move3count << endl;
        cout << "Move4 count: " << move4count << endl;
        cout << "Accept count: " << accpetCount << endl;
        cout << "Reject count: " << rejectCount << endl;
        cout << "Step: " << step << endl;
        cout << "--------------------------" << endl;
        outputFile.close();
        timer.stop();
    }

    inline void getNewSolution(int operation, pair<int, int> &preresult, bool isBack)
    {
        switch (operation)
        {
        // swap two blocks only in X
        case 0:
        {
            // 在水平序列中交換兩個區塊的代碼
            // cout << "swap two blocks only in X" << endl;
            int vertexIndex1, vertexIndex2;
            if(isBack)
            {
                vertexIndex1 = preresult.first;
                vertexIndex2 = preresult.second;
            }
            else
            {
                vertexIndex1 = getRandomBlock(graphH->size());
                vertexIndex2 = getRandomBlock(graphH->size());
                preresult = {vertexIndex1, vertexIndex2};
                //cout << "preresult: " << preresult.first << " " << preresult.second << endl;
            }
            //cout << "vertexIndex1: " << vertexIndex1 << " vertexIndex2: " << vertexIndex2 << endl;
            //cout << "vertexIndex1: " << vertexIndex1 << " vertexIndex2: " << vertexIndex2 << endl;
            if(vertexIndex1 != vertexIndex2)
            {
                //cout << "Before clear" << graphH->getEdgeWeight(0, 4) << endl;
                graphH->swapXVertex(vertexIndex1, vertexIndex2);
                graphV->swapXVertex(vertexIndex1, vertexIndex2);
                graphH->maintainH(vertexIndex1, vertexIndex2);
                graphV->maintainV(vertexIndex1, vertexIndex2);

                //cout << "After clear" << graphH->getEdgeWeight(0, 4) << endl;
                //cout << "--------------------------" << endl;
                
            }
            
            move1count++;
            break;
        }

        // swap two blocks only in Y
        case 1:
        {
            // 在垂直序列中交換兩個區塊的代碼
            // cout << "swap two blocks only in Y" << endl;
            int vertexIndex1, vertexIndex2;
            if(isBack)
            {
                vertexIndex1 = preresult.first;
                vertexIndex2 = preresult.second;
            }
            else
            {
                vertexIndex1 = getRandomBlock(graphH->size());
                vertexIndex2 = getRandomBlock(graphH->size());
                preresult = {vertexIndex1, vertexIndex2};
                //cout << "preresult: " << preresult.first << " " << preresult.second << endl;
            }
            //cout << "vertexIndex1: " << vertexIndex1 << " vertexIndex2: " << vertexIndex2 << endl;
            if(vertexIndex1 != vertexIndex2)
            {
                //cout << "Before clear" << graphH->getEdgeWeight(0, 4) << endl;
                graphH->swapYVertex(vertexIndex1, vertexIndex2);
                graphV->swapYVertex(vertexIndex1, vertexIndex2);
                graphH->maintainH(vertexIndex1, vertexIndex2);
                graphV->maintainV(vertexIndex1, vertexIndex2);

                //cout << "After clear" << graphH->getEdgeWeight(0, 4) << endl;
                //cout << "--------------------------" << endl;
            }
            move2count++;
            break;
        }

        // swap two blocks in both sequence
        case 2:
        {
            // 同時在水平和垂直序列中交換兩個區塊的代碼
            // cout << "swap two blocks in both sequence" << endl;
            int vertexIndex1, vertexIndex2;
            if(isBack)
            {
                vertexIndex1 = preresult.first;
                vertexIndex2 = preresult.second;
            }
            else
            {
                vertexIndex1 = getRandomBlock(graphH->size());
                vertexIndex2 = getRandomBlock(graphH->size());
                preresult = {vertexIndex1, vertexIndex2};
                //cout << "preresult: " << preresult.first << " " << preresult.second << endl;
            }

            if(vertexIndex1 != vertexIndex2)
            {
                graphH->swapXVertex(vertexIndex1, vertexIndex2);
                graphV->swapXVertex(vertexIndex1, vertexIndex2);
                graphH->swapYVertex(vertexIndex1, vertexIndex2);
                graphV->swapYVertex(vertexIndex1, vertexIndex2);
                graphH->maintainH(vertexIndex1, vertexIndex2);
                graphV->maintainV(vertexIndex1, vertexIndex2);
            }
            move3count++;
            break;
        }

        // rotate a block
        case 3:
        {
            // 隨機選擇一個頂點的索引
            int vertexIndex;
            if(isBack)
            {
                vertexIndex = preresult.first;
            }
            else
            {
                vertexIndex = getRandomBlock(graphH->size());
                preresult = {vertexIndex, 0};
                //cout << "preresult: " << preresult.first << " " << preresult.second << endl;
            }
            //cout << "vertexIndex: " << preresult.first << endl;
            graphH->rotateBlock(vertexIndex, 0);
            graphV->rotateBlock(vertexIndex, 1);
            move4count++;
            break;
        }
        }
    }

    void storeGlobalBestGraph()
    {
        for(int i = 0; i < N; i++)
        {
            coordinateX[i] = graphH->calculateMaxTotalEdgeWeight(N, i);
            coordinateY[i] = graphV->calculateMaxTotalEdgeWeight(N, i);
        }
    }

    vector<int> getCoordinateX()
    {
        return coordinateX;
    }

    vector<int> getCoordinateY()
    {
        return coordinateY;
    }

    int getGlobalBestH()
    {
        return GlobalBestH;
    }

    int getGlobalBestV()
    {
        return GlobalBestV;
    }
};

inline int getRandomOperation()
{
    // 使用當前時間來設置隨機種子，以確保每次程序運行時生成的隨機數都不同
    static std::random_device rd;
    unsigned int seed1, seed2;
    seed1 = rd();
    seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed1 + seed2);

    // 生成0到3之間的隨機數
    std::uniform_int_distribution<int> dis(0, 3);
    return dis(gen);
}

inline int getRandomBlock(int numBlock)
{
    // 使用當前時間來設置隨機種子，以確保每次程序運行時生成的隨機數都不同
    static std::random_device rd;
    unsigned int seed1, seed2;
    seed1 = rd();
    seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed1 + seed2);

    // 生成0到numBlock之間的隨機數
    std::uniform_int_distribution<int> dis(0, numBlock - 1);
    return dis(gen);
}

inline float getRandomProbability()
{
    static std::random_device rd;
    unsigned int seed1, seed2;
    seed1 = rd();
    seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed1 + seed2);

    // 生成0到RAND_MAX之間的隨機整數，然後將其除以RAND_MAX來獲得0到1之間的隨機浮點數
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

#endif // SIMULATED_ANNEALING_HPP