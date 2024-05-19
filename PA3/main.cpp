#include <iostream>
#define IL_STD
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <cstdlib>

#include <ilcplex/ilocplex.h>
#include "InputDataParse.hpp"
#include "Timer.hpp"

using namespace std;
ILOSTLBEGIN

vector<vector<int>> findVertexeNeighbors(vector<vector<int>> data);

vector<vector<int>>findVertexeNeighbors(vector<vector<int>> data)
{
    vector<vector<int>> vertexNeighborList = vector<vector<int>>(data.size());

    for(size_t i = 0; i < data.size(); i++)
    {
        for(size_t j = i; j < data.size(); j++)
        {
            // check if the two vertices are neighbors(distance<=2)
            if( abs(data[i][1] - data[j][1]) <= 2 && abs(data[i][2] - data[j][2]) <= 2 && i != j)
            {
                // add the two vertices to the neighbor list
                vertexNeighborList[i].push_back(j);
                //cout << "Vertex " << i << " and " << j << " are neighbors." << endl;
            }
        }
    }

    return vertexNeighborList;
}

int main( int argc, char **argv )
{
    Timer timer;
    timer.start();

    // convert argv to string
    string arg1 = argv[1];
    string arg2 = argv[2];

    cout << "Input file: " << arg1 << endl;
    cout << "Output file: " << arg2 << endl;

    ifstream inputFile(arg1);
    if (!inputFile.is_open())
    {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    inputFile.close();

    InputDataParse inputDataParse;
    inputDataParse.parseInputData(arg1);
    //inputDataParse.printParsedData();

    vector<vector<int>> parsedData = inputDataParse.getParsedData();
    vector<vector<int>> vertexNeighborList = findVertexeNeighbors(parsedData);
    
    IloEnv env;
    try {
        IloModel model(env);
        IloCplex cplex(model);

        //cplex.setParam(IloCplex::Param::TimeLimit, 500); // set time limit to 600s

        // Define decision variables
        // Define decision variables
        IloIntVarArray xi1(env, parsedData.size(), 0, 1); // xi1 is the color bit 1 for each vertex
        IloIntVarArray xi2(env, parsedData.size(), 0, 1); // xi2 is the color bit 2 for each vertex

        IloIntVarArray cij1(env, parsedData.size() * parsedData.size(), 0, 1); // cij1 is the color conflict bit 1 for each pair of vertices
        IloIntVarArray cij2(env, parsedData.size() * parsedData.size(), 0, 1); // cij2 is the color conflict bit 2 for each pair of vertices
        IloIntVarArray cij(env, parsedData.size() * parsedData.size(), 0, 1);  // cij is the color conflict bit for each pair of vertices

        //cout <<"size of parsedData: " << parsedData.size() << endl;
        //cout <<"size of vertexNeighborList: " << vertexNeighborList.size() << endl;
        
        // Define constraints for each vertex
        for (size_t i = 0; i < parsedData.size(); i++)
        {
            // Define constraints for three available colors for each vertex
            model.add(xi1[i] + xi2[i] <= 1);
            // Define constraints for conflict detection
            for (size_t j = 0; j < vertexNeighborList[i].size(); j++)
            {
                int y = vertexNeighborList[i][j];
                // Define constraints for each pair of vertices
                if(i != y)
                {
                    // Define constraints for each pair of vertices
                    // Define constraints for each pair of vertices
                    IloConstraintArray constraints(env);
                    constraints.add(xi1[i] + xi1[y] - cij1[i * parsedData.size() + y] <= 1);
                    constraints.add(xi2[i] + xi2[y] - cij2[i * parsedData.size() + y] <= 1);
                    constraints.add(1 - xi1[i] + 1 - xi1[y] - cij1[i * parsedData.size() + y] <= 1);
                    constraints.add(1 - xi2[i] + 1 - xi2[y] - cij2[i * parsedData.size() + y] <= 1);
                    constraints.add(cij1[i * parsedData.size() + y] + cij2[i * parsedData.size() + y] - cij[i * parsedData.size() + y] <= 1);

                    model.add(constraints);

                    //cout << "Constraints added for vertex " << i << " and " << y << endl;
                }
            }
        }

        //cout << "Constraints added." << endl;

        // Objective function (num of cij == 1)
        IloExpr obj(env);
        for(size_t i = 0; i < parsedData.size(); i++)
        {
            for(size_t j = 0; j < vertexNeighborList[i].size(); j++)
            {
                int y = vertexNeighborList[i][j];
                //cout << "i: " << i << " y: " << y << endl;
                if(i != y)
                {
                    //cout << "cij[" << i << "][" << y << "]: " << cij[i * parsedData.size() + y] << endl;
                    obj = obj + cij[i * parsedData.size() + y];
                }
            }
        }

        model.add(IloMinimize(env, obj));
        obj.end();

        // Solve the problem
        cplex.solve();

        cout << "Objective value: " << cplex.getObjValue() << endl;

        // Output the results
        ofstream outputFile(arg2);
        // output num of cij == 1 (conflict)
        outputFile << cplex.getObjValue() << endl;
        // output the color of each pattern mask(1, 2, 3)
        for(size_t i = 0; i < parsedData.size(); i++)
        {
            if(cplex.getValue(xi1[i]) == 0 && cplex.getValue(xi2[i]) == 0)
            {
                outputFile << i << " " << 1 << endl;
            }
            if(cplex.getValue(xi1[i]) == 0 && cplex.getValue(xi2[i]) == 1)
            {
                outputFile << i << " " << 2 << endl;
            }
            if(cplex.getValue(xi1[i]) == 1 && cplex.getValue(xi2[i]) == 0)
            {
                outputFile << i << " " << 3 << endl;
            }
        }

    } catch (IloException& ex) {
        std::cerr << "Error: " << ex << std::endl;
    }
    env.end();
    
    timer.stop();
    cout << "Time elapsed: " << timer.elapsed() << "ms" << endl;

    return 0;
}
