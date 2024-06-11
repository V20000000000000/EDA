// Description: main function for the project

#include <iostream>
#include <fstream>
#include <vector>

#include "InputDataParse.hpp"

int main()
{
    InputDataParse idp("testcase", "case4");
    idp.parse();
    return 0;
}

