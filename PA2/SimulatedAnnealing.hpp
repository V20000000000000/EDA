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

#include "HVGraph.hpp"
#include "Block.hpp"
#include "Timer.hpp"

using namespace std;

int getRandomOperation(double prob0, double prob1, double prob2, double prob3);
int getRandomBlock(int numBlock);
float getRandomProbability();

class SimulatedAnnealing
{
private:
    double temperature;
    double coolingRate;
    double probabilityThreshold;
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
    int step;
    HVGraph<Block *, int> *graphH;
    HVGraph<Block *, int> *graphV;
    vector<int> coordinateX;
    vector<int> coordinateY;
    vector<int> blockWidth;
    vector<int> blockHeight;

    pair<int, int> preresult;

public:
    // Constructor
    SimulatedAnnealing(HVGraph<Block *, int> *graphH, HVGraph<Block *, int> *graphV) : graphH(graphH), graphV(graphV), coordinateX(graphH->size()), 
    coordinateY(graphH->size()), blockWidth(graphH->size(), 0), blockHeight(graphH->size(), 0)
    {
        cout << "SimulatedAnnealing constructor" << endl;
    }
    // Destructor
    ~SimulatedAnnealing() 
    {
        cout << "SimulatedAnnealing destructor" << endl;
    }

    void run()
    {
        // Initialize the timer
        ofstream outputLogFile("log.txt");
        Timer timer;
        timer.start();
        // Initialize the temperature
        temperature = 100;
        step = 0;

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
        //cout << "temperature" << temperature << endl;
        bool run = true;
        // Loop until the temperature is zero
        while (temperature > 0.00001 && run)
        {
            for (int i = P; i >= 1; i--)
            {
                //cout << "Temperature: " << temperature << " ";
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
                int operation;
                operation = getRandomOperation(0.25, 0.25, 0.25, 0.25);
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

                        if(N > 50 && temperature > 2)
                        {
                            if(temperature < 80 && temperature > 70)
                                storeGlobalBestGraph();
                            else
                                continue;
                        }else
                        {
                            storeGlobalBestGraph();
                        }
                    }

                // If the new solution is better, accept it
                if (currentCost < bestCost)
                {
                    bestCost = currentCost;
                }
                else // If the new solution is worse, accept it with a probability
                {
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
            storeGlobalBestGraph();
            cout << "step: " << step << " ";
            cout << "Best solution: " << GlobalBestCost << " " << "temperature " <<
            temperature << " time: " << timer.elapsed() << endl;
            cout << "probabilityThreshold: " << probabilityThreshold << endl;
            outputLogFile << "step: " << step << " ";
            outputLogFile << "Best solution: " << GlobalBestCost << endl;
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
        outputLogFile.close();
        timer.stop();
    }

    void getNewSolution(int operation, pair<int, int> &preresult, bool isBack)
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
        if(step != 0)
        {
            for(int i = 0; i < N; i++)
            {
                coordinateX[i] = graphH->calculateMaxTotalEdgeWeight(N, i);
                coordinateY[i] = graphV->calculateMaxTotalEdgeWeight(N, i);
                blockWidth[i] = graphH->getVertexProperty(i).value->getWidth();
                blockHeight[i] = graphV->getVertexProperty(i).value->getHeight();
            }
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

    vector<int> getBlockWidth()
    {
        return blockWidth;
    }

    vector<int> getBlockHeight()
    {
        return blockHeight;
    }

    int getGlobalBestH()
    {
        return GlobalBestH;
    }

    int getGlobalBestV()
    {
        return GlobalBestV;
    }

    int getSize()
    {
        return N;
    
    }
};

#include <random>
#include <chrono>

inline int getRandomOperation(double prob0, double prob1, double prob2, double prob3)
{
    // 使用當前時間來設置隨機種子，以確保每次程序運行時生成的隨機數都不同
    static std::random_device rd;
    unsigned int seed1, seed2;
    seed1 = rd();
    seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed1 + seed2);

    // 計算總機率，用於歸一化
    double totalProb = prob0 + prob1 + prob2 + prob3;

    // 生成0到1之間的隨機數，用於決定哪個數字生成
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    double randNum = dis(gen);

    // 根據隨機數決定生成哪個數字
    if (randNum < prob0 / totalProb) {
        return 0;
    } else if (randNum < (prob0 + prob1) / totalProb) {
        return 1;
    } else if (randNum < (prob0 + prob1 + prob2) / totalProb) {
        return 2;
    } else {
        return 3;
    }
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
