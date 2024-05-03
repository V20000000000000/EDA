#ifndef INPUT_DATA_PARSE_HPP
#define INPUT_DATA_PARSE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <utility>

using namespace std;

class InputDataParse
{
    public:
        int numBlocks;
        float minAspectRatio;
        float maxAspectRatio;
        vector<pair<int, int>> blocksList; // Store block dimensions
        vector<string> blockNamesList; // Store block names

        InputDataParse(string inputFileName)    // Constructor
        {
            cout << "start initialSolution of " << inputFileName << endl;
            ifstream inputFile(inputFileName);
            while(!inputFile.eof()) 
            {
                string inputLine;
                stringstream ss;
                string token;

                ss.str("");
                ss.clear();

                getline(inputFile, inputLine);
                ss.str(inputLine);
                
                while (ss >> token)
                {
                    if (token == "NumBlocks")   // Read number of blocks
                    {
                        string x;
                        ss >> x;
                        numBlocks = stoi(x);
                    }
                    else if (token == "MinAspectRatio")    // Read min aspect ratio
                    {
                        string x;
                        ss >> x;
                        minAspectRatio = stof(x);
                    }
                    else if (token == "MaxAspectRatio")    // Read max aspect ratio
                    {
                        string x;
                        ss >> x;
                        maxAspectRatio = stof(x);
                    }
                    else    // Read block dimensions
                    {
                            int numX, numY;
                            string x, y;
                            ss >> x >> y;
                            numX = stoi(x);
                            numY = stoi(y);
                            blocksList.push_back(make_pair(numX, numY));
                            blockNamesList.push_back(token);
                    
                    }
                }
            }
            inputFile.close();
            cout << "finish initialSolution of " << inputFileName << endl;
        };

        ~InputDataParse(){}; // Destructor

        int getNumBlocks()
        {
           return numBlocks;
        };

        float getMinAspectRatio()
        {
            return minAspectRatio;
        };

        float getMaxAspectRatio()
        {
            return maxAspectRatio;
        };

        vector<pair<int, int>> getBlocksList() 
        {
            return blocksList;
        }

        vector<string> getBlocksNameList() 
        {
            return blockNamesList;
        }

        pair<int, int> getBlock(int index)
        {
            return blocksList[index];
        }

        int getBlockWidth(int index)
        {
            return blocksList[index].first;
        }

        int getBlockHeight(int index)
        {
            return blocksList[index].second;
        }
};

#endif