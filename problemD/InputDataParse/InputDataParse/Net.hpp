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
    vector<string> rx;
    vector<string> tx;
    int num;
    map <string, pair<pair<int, int>, pair<int, int>>> must_through;
    map <string, pair<pair<int, int>, pair<int, int>>> hmft_must_through;

public:
    Net(int id) : id(id) {}

    int getId() const
    {
        return id;
    }

    vector<string> getRx() const
    {
        return rx;
    }

    vector<string> getTx() const
    {
        return tx;
    }

    int getNum() const
    {
        return num;
    }

    map <string, pair<pair<int, int>, pair<int, int>>> getMustThrough() const
    {
        return must_through;
    }

    map <string, pair<pair<int, int>, pair<int, int>>> getHmftMustThrough() const
    {
        return hmft_must_through;
    }

    void addRx(string p)
    {
        rx.push_back(p);
    }

    void addTx(string p)
    {
        tx.push_back(p);
    }

    void setNum(int n)
    {
        num = n;
    }

    void addMustThrough(string s, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
    {
        must_through.emplace(s, make_pair(make_pair(x0, y0), make_pair(x1, y1)));
    }

    void addHmftMustThrough(string s, int x0, int y0, int x1, int y1)
    {
       hmft_must_through.emplace(s, make_pair(make_pair(x0, y0), make_pair(x1, y1)));
    }
};

#endif
    

