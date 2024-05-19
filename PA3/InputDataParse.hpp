#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class InputDataParse {
private:
    vector<vector<int>> parsedData;
    int layoutDimension1;
    int layoutDimension2;
    int numHoles;
public:
    InputDataParse() : layoutDimension1(0), layoutDimension2(0), numHoles(0) {} // 默认构造函数
    ~InputDataParse() = default; // 默认析构函数
    void parseInputData(const string& fileName); // 添加 const 引用
    void printParsedData() const; // 添加 const 成员函数
    
    vector<vector<int>> getParsedData() const; // 修正返回类型并添加 const 成员函数
    int getNumHoles();
    int getDim1();
    int getDim2();
};

// 在类外部实现成员函数
void InputDataParse::parseInputData(const string& fileName) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return;
    }

    string line;
    bool firstLine = true;
    bool secondLine = true;
    while (getline(inputFile, line)) {
        istringstream iss(line);

        if (firstLine) {
            string layout;
            iss >> layout >> layoutDimension1 >> layoutDimension2;
            firstLine = false;
        } else if (secondLine) {
            string numHolesStr;
            iss >> numHolesStr >> numHoles;
            secondLine = false;
        } else {
            int holeId, x, y;
            iss >> holeId >> x >> y;
            parsedData.push_back({holeId, x, y});
        }
    }

    inputFile.close();
}


int InputDataParse::getNumHoles()
{
   return numHoles;
}

int InputDataParse::getDim1()
{
   return layoutDimension1;
}

int InputDataParse::getDim2()
{
   return layoutDimension2;
}

void InputDataParse::printParsedData() const {
    cout << "Layout dimensions: " << layoutDimension1 << "x" << layoutDimension2 << endl;
    cout << "Number of holes: " << numHoles << endl;
    cout << "Hole positions:" << endl;
    for (const auto& row : parsedData) {
        for (const auto& num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
}


vector<vector<int>> InputDataParse::getParsedData() const {
    return parsedData;
}

