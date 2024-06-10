#pragma once

#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string> split(string &str, char delim)
{
    vector<string> res;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim))
    {
        res.push_back(token);
    }
    return res;
}
