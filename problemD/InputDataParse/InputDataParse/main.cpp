// Description: main function for the project

#include <iostream>
#include <fstream>
#include <vector>

#include "InputDataParse.hpp"

int main()
{
    InputDataParse idp("testcase", "case4");
    idp.parse();
    cout << "---------------------\n";
    idp.printBlockData();
    cout << "---------------------\n";
    //idp.printNetData();
    return 0;
}

