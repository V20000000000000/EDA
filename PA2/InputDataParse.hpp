#ifndef INPUT_DATA_PARSE_HPP
#define INPUT_DATA_PARSE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

class InputDataParse
{
    public:
        int numBlocks;
        float minAspectRatio;
        float maxAspectRatio;
        vector<map<string, vector<int>>> blocksList;

        InputDataParse(string inputFileName)    // Constructor
        {
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
                        string prefix = inputLine.substr(0, 5);
                        if (prefix == "block")
                        {
                            int numX, numY;
                            string x, y;
                            map<string, vector<int>> block;
                            ss >> x >> y;
                            numX = stoi(x);
                            numY = stoi(y);
                            block[token].push_back(numX); // Add block dimensions to block map
                            block[token].push_back(numY);
                            blocksList.push_back(block);
                        }
                    }
                }
            }
        };
        ~InputDataParse(){}; // Destructor

        void getNumBlocks()
        {
            cout << "Number of blocks: " << numBlocks << endl;
        };

        void getMinAspectRatio()
        {
            cout << "Minimum aspect ratio: " << minAspectRatio << endl;
        };

        void getMaxAspectRatio()
        {
            cout << "Maximum aspect ratio: " << maxAspectRatio << endl;
        };

        void getBlocksList() 
        {
            for (const auto& block : blocksList) 
            {
                for (const auto& pair : block) 
                {
                    cout << pair.first << " " << pair.second[0] << " " << pair.second[1] << std::endl;
                }
            }
        }

        void getBlock(int index)
        {
                for (const auto& pair : blocksList[index]) 
                {
                    if (pair.first == "block_" + to_string(index))
                    {
                        cout << pair.first << " " << pair.second[0] << " " << pair.second[1] << endl;
                    }
                }
        }

        void getBlockWidth(int index)
        {
            for (const auto& pair : blocksList[index]) 
                {
                    if (pair.first == "block_" + to_string(index))
                    {
                        cout << pair.first << " width: " << pair.second[0] << endl;
                    }
                }
        }

        void getBlockHeight(int index)
        {
            for (const auto& pair : blocksList[index]) 
                {
                    if (pair.first == "block_" + to_string(index))
                    {
                        cout << pair.first << " height: " << pair.second[1] << endl;
                    }
                }
        }
};

#endif