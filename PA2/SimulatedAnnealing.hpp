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

#include "HVgraph.hpp"
#include "Block.hpp"

using namespace std;

int getRandomOperation();
int getRandomBlock(int numBlock);
float getRandomProbability();

class SimulatedAnnealing {
private:
    double temperature;
    double coolingRate;
    int MaxDistanceH;
    int MaxDistanceV;
    int initialDistanceH;
    int initialDistanceV;
    int initialCost;
    int bestCost;
    int currentCost;
    int move1count;
    int move2count;
    int move3count;
    int move4count;
    int accpetCount;
    int rejectCount;
    int P;
    HVGraph <Block, int> *graphH;
    HVGraph <Block, int> *graphV;

public:
    // Constructor
    SimulatedAnnealing(HVGraph<Block, int> *graphH, HVGraph<Block, int> *graphV): graphH(graphH), graphV(graphV){}
    // Destructor
    ~SimulatedAnnealing(){}

    void run()
    {
        // Initialize the temperature
        temperature = 10000;
        int step = 0;

        // Set cooling rate
        coolingRate = 0.85;

        // Initialize the best cost

        // Initialize the current cost
        currentCost = 0;

        // initialize the move count
        move1count = 0;
        move2count = 0;
        move3count = 0;
        move4count = 0;
        accpetCount = 0;
        rejectCount = 0;

        P = 1 * graphH->size();

        // Get the chip width and height
        cout << "--------------------------" << endl;
        MaxDistanceH = graphH->calculateMaxTotalEdgeWeight(graphH->size(), graphH->size() + 1);
        MaxDistanceV = graphV->calculateMaxTotalEdgeWeight(graphV->size(), graphV->size() + 1);
        cout << "--------------------------" << endl;
        cout << "MaxDistanceH: " << MaxDistanceH << " MaxDistanceV: " << MaxDistanceV << endl;
        cout << "--------------------------" << endl;

        int preDistH = MaxDistanceH;
        int preDistV = MaxDistanceV;
        
        // Initialize the best cost
        initialCost = max(MaxDistanceH, MaxDistanceV);
        initialDistanceH = MaxDistanceH;
        initialDistanceV = MaxDistanceV;
        cout << "Initial DistH: " << initialDistanceH << endl;
        cout << "Initial DistV: " << initialDistanceV << endl;
        bestCost = max(initialDistanceH, initialDistanceV);

        // Loop until the temperature is zero
        while (temperature > 1)
        {
            for (int i = P; i >= 1; i--)
            {
                preDistH = MaxDistanceH;
                preDistV = MaxDistanceV;
                cout << "preDistH: " << preDistH << " ";
                cout << "preDistV: " << preDistV << endl;
                
                // random select a operation
                int operation = getRandomOperation();

                // Create a new solution
                getNewSolution(operation);

                // Calculate the cost of the new solution
                //update the source set and target set
                MaxDistanceH = graphH->calculateMaxTotalEdgeWeight(graphH->size(), graphH->size() + 1);
                MaxDistanceV = graphV->calculateMaxTotalEdgeWeight(graphV->size(), graphV->size() + 1);
                //cout << "MaxDistanceH: " << MaxDistanceH << " MaxDistanceV: " << MaxDistanceV << endl;
                currentCost = max(MaxDistanceH, MaxDistanceV);
                

                // Calculate the cost difference
                int costDifference = currentCost - bestCost;

                // If the new solution is better, accept it
                if(currentCost < bestCost)
                {
                    bestCost = currentCost;
                }
                else    // If the new solution is worse, accept it with a probability
                {
                    double probabilityThreshold = exp(-costDifference / temperature);
                    double probability = getRandomProbability();

                    if (probability <= probabilityThreshold)
                    {
                        // Accept the new solution
                        bestCost = currentCost;
                        accpetCount++;
                    }
                    else
                    {
                        getNewSolution(operation);  // Back to the original solution
                        MaxDistanceH = preDistH;
                        MaxDistanceV = preDistV;
                        cout << "reject" << endl;
                        rejectCount++;
                    }
                }
                // If the new solution is the best, update the best solution
            }
            step = step + 1;
            // Cool the temperature
            cout << "step: " << step << " ";
            cout << "Best solution: " << bestCost << endl;
            //cout << "Temperature: " << temperature << endl;
            cout << "MaxDistanceH: " << MaxDistanceH << " MaxDistanceV: " << MaxDistanceV << endl;
            temperature = temperature * coolingRate;
        }

        // Output the best solution
        cout << "Initial solution: " << initialCost << endl;
        cout << "DistanceH: " << initialDistanceH << " DistanceV: " << initialDistanceV << endl;
        cout << "Best solution: " << bestCost << endl;
        cout << "DistanceH: " << MaxDistanceH << " DistanceV: " << MaxDistanceV << endl;
        cout << "Move1 count: " << move1count << endl;
        cout << "Move2 count: " << move2count << endl;
        cout << "Move3 count: " << move3count << endl;
        cout << "Move4 count: " << move4count << endl;
        cout << "Accept count: " << accpetCount << endl;
        cout << "Reject count: " << rejectCount << endl;
    }

    HVGraph<Block, int> *getHorizontalGraph() const
    {
        return graphH;
    }

    HVGraph<Block, int> *getVerticalGraph() const
    {
        return graphV;
    }

    void getNewSolution(int operation)
    {
        switch (operation)
        {
            // swap two blocks only in X
            case 0:
            {
                // 在水平序列中交換兩個區塊的代碼
                //cout << "swap two blocks only in X" << endl;
                move1count++;
                break;
            }

            // swap two blocks only in Y
            case 1:
            {
                // 在垂直序列中交換兩個區塊的代碼
                //cout << "swap two blocks only in Y" << endl;
                move2count++;
                break;
            }

            // swap two blocks in both sequence
            case 2:
            {
                // 同時在水平和垂直序列中交換兩個區塊的代碼
                //cout << "swap two blocks in both sequence" << endl;
                move3count++;
                break;
            }

            // rotate a block
            case 3:
            {
                // 隨機選擇一個頂點的索引
                int vertexIndex = getRandomBlock(graphH->size());

                //顯示block原本的長寬
                //cout << "Block_" << vertexIndex << " width: " << graphH->getVertexProperty(vertexIndex).value.getWidth() << " height: " << graphV->getVertexProperty(vertexIndex).value.getHeight() << endl;
                
                // 旋轉區塊
                //cout << "rotate a block" << endl;

                graphH->rotateBlock(vertexIndex);
                graphV->rotateBlock(vertexIndex);

                graphH->recalculateVertexEdgeWeight(vertexIndex, 0);
                graphV->recalculateVertexEdgeWeight(vertexIndex, 1);

                //顯示block旋轉後的長寬
                //cout << "Block_" << vertexIndex << " width: " << graphH->getVertexProperty(vertexIndex).value.getWidth() << " height: " << graphV->getVertexProperty(vertexIndex).value.getHeight() << endl;
                
                move4count++;
                break;
            }
        }
    }
};

int getRandomOperation()
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

int getRandomBlock(int numBlock)
{
    // 使用當前時間來設置隨機種子，以確保每次程序運行時生成的隨機數都不同
    static std::random_device rd;
    unsigned int seed1, seed2;
    seed1 = rd();
    seed2 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed1 + seed2);
    
    // 生成0到numBlock之間的隨機數
    std::uniform_int_distribution<int> dis(0, numBlock-1);
    return dis(gen);
}

float getRandomProbability()
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