// Parse Net data from /testcase/caseX/caseX_def/caseX.json
/*
      {
    "ID":2305,
    "TX":"REGION_128",
    "RX":["BLOCK_6","BLOCK_6","BLOCK_11"],
    "NUM":6,
    "MUST_THROUGH":{},
    "HMFT_MUST_THROUGH":{},
    "TX_COORD":[1580.000,2661.000],
    "RX_COORD":[[308.000,197.000],[308.000,197.000],[1240.000,1539.899]]
  },
*/

#ifndef NET_HPP
#define NET_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utility> // for std::pair

using namespace std;

class Net
{
private:
    int id;
    string net_name;
    vector<pair<int, int>> rx;
    vector<pair<int, int>> tx;
    int num;
    vector<pair<int, int>> must_through;
    vector<pair<int, int>> hmft_must_through;

public:
    Net(int id) : id(id) {}

    int getId() const
    {
        return id;
    }

    void setNetName(const string &name)
    {
        net_name = name;
    }

    void addRx(int x, int y)
    {
        rx.push_back(make_pair(x, y));
    }

    void addTx(int x, int y)
    {
        tx.push_back(make_pair(x, y));
    }

    void setNum(int n)
    {
        num = n;
    }

    void addMustThrough(int x, int y)
    {
        must_through.push_back(make_pair(x, y));
    }

    void addHmftMustThrough(int x, int y)
    {
        hmft_must_through.push_back(make_pair(x, y));
    }
};

#endif
    

