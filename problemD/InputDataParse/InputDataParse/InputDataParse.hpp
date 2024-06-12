// InputDataParse class definition
// Parsing blk data from /testcase/caseX/caseX_def/blk_id.def and caseX_cfg.json
// Parsing net data from /testcase/caseX/caseX_def/net_id.def and caseX.json
// class input: caseX

#ifndef INPUT_DATA_PARSE_HPP
#define INPUT_DATA_PARSE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility> // for std::pair
#include "include\nlohmann\json.hpp"
#include "include\utility\split.hpp"

#include "Blk.hpp"
#include "Net.hpp"
#include "Region.hpp"

using namespace std;

Direction stringToDirection(const string &str);
string DirectionToString(Direction dir);

/**
 * @brief InputDataParse class
 *
 */
class InputDataParse
{
private:
    string base_dir, case_name;
    // block container
    vector<Blk> allBlocks;
    // net container
    vector<Net> allNets;
    // region container
    vector<Region> allRegions;
    // unit distance in micros
    int unit_distance_microns;
    // block num
    int block_num;
    int region_num;
    int net_num;

    /**
     * @brief parse chip_top.def
     *
     * @return void
     */
    void parseChipTop();

    /**
     * @brief parse caseX_cfg.json
     *
     * @param case_name: name of the testcase
     * @param block_num: number of blocks in the testcase
     *
     * @return void
     */
    void parseBlockData(int block_num);

    /**
     * @brief parse caseX_cfg.json
     *
     * @param case_name: name of the testcase
     * @param block_num: number of blocks in the testcase
     *
     * @return void
     */
    void parseNetData(const string &case_name);

    /**
     * @brief parse caseX_cfg.json
     *
     * @param case_name: name of the testcase
     * @param block_num: number of blocks in the testcase
     *
     * @return void
     */
    void writeFile(const string &case_name);

public:
    InputDataParse(const string &base_dir, const string &case_name) : base_dir(base_dir), case_name(case_name) 
    {
        // read chip_top.def from file
        ifstream fin(base_dir + "/" + case_name + "/" + case_name + "_def/chip_top.def");
        cout << "Reading chip_top.def" << endl;
        string line;
        vector<string> tokens;
        while (getline(fin, line))
        {
            tokens = split(line, ' ');
            cout << line << endl;
            if(line.empty()) continue;
            if (tokens[0] == "COMPONENTS")
            {
                block_num = stoi(tokens[1]);
            }
            else if (tokens[0] == "REGIONS")
            {
                region_num = stoi(tokens[1]);
                break;
            }
        }
        cout << "Block num: " << block_num << endl;
        cout << "Region num: " << region_num << endl;
        // build a vector of Blk objects
        for (int i = 0; i < block_num; i++)
        {
            allBlocks.push_back(Blk(i));
        }

        for (int i = 0; i < region_num; i++)
        {
            allRegions.push_back(Region(i));
        }

        fin.close();
    }

    void printBlockData();
    void printNetData();

    void parse()
    {
        // TODO: parse chip_top.def
        parseChipTop();
        // TODO: parse block data
        parseBlockData(block_num);
        // TODO: parse caseX_cfg.json
        // TODO: parse net data(caseX.json)
        parseNetData(case_name);
        cout << "Parsing complete" << endl;

        writeFile(case_name);
    }
};

void InputDataParse::parseChipTop()
{
    // read chip_top.def from file
    ifstream fin(base_dir + "/" + case_name + "/" + case_name + "_def/chip_top.def");
    cout << "Reading chip_top.def" << endl;
    string line;
    vector<string> tokens;
    while (getline(fin, line))
    {
        cout << line << endl;
        tokens = split(line, ' ');
        if(line.empty())
        {
            continue;
        }
        else if (tokens[0] == "UNITS")
        {
            unit_distance_microns = stoi(tokens[3]);
        }
        else if (tokens[0] == "COMPONENTS")
        {
            block_num = stoi(tokens[1]);
            
        }
        else if (tokens[0] == "-" && tokens[3] == "+")
        {
            string blk_name = tokens[1];

             // Find the position of the underscore
            size_t pos = blk_name.find('_');
            
            string number_str;
            int number;

            if (pos != string::npos) {
                // Extract the number part after the underscore
                number_str = blk_name.substr(pos + 1);
                number = stoi(number_str);
            }

            // If you need to extract coordinates as well:
            int x_coord = stoi(tokens[6]);
            int y_coord = stoi(tokens[7]);
            string orientation = tokens[9];

            // Access the block using a reference to modify it directly in the vector
            Blk &block = allBlocks[number];
            block.setLocX(x_coord);
            block.setLocY(y_coord);
            block.setDirection(stringToDirection(orientation));
            block.setBlockName(blk_name);
            block.setBlockIdName(tokens[2]);

            // // Example output
            // cout << "Block name: " << blk_name << endl;
            // cout << "X coordinate: " << x_coord << endl;
            // cout << "Y coordinate: " << y_coord << endl;
            // cout << "Orientation: " << orientation << endl;

            // // Example output
            // cout << "Block name: " << allBlocks[number].getBlockName() << endl;
            // cout << "X coordinate: " << allBlocks[number].getLocX() << endl;
            // cout << "Y coordinate: " << allBlocks[number].getLocY() << endl;
            // cout << "Orientation: " << allBlocks[number].getDirection() << endl;
        }
        else if (tokens[0] == "REGIONS")
        {
            region_num = stoi(tokens[1]);
        }
        else if (tokens[0] == "-" && tokens[2] == "(")
        {
            string region_name = tokens[1];

             // Find the position of the underscore
            size_t pos = region_name.find('_');
            
            string number_str;
            int number;

            if (pos != string::npos) {
                // Extract the number part after the underscore
                number_str = region_name.substr(pos + 1);
                number = stoi(number_str);
            }

            Region &region = allRegions[number];
            region.setName(region_name);
            region.setX0(stoi(tokens[3]));
            region.setY0(stoi(tokens[4]));
            region.setX1(stoi(tokens[7]));
            region.setY1(stoi(tokens[8]));

            // Store the region in a vector
            allRegions.push_back(Region(number));

            //// Example output
            // cout << "Region name: " << region_name << endl;
            // cout << "X0 coordinate: " << stoi(tokens[3]) << endl;
            // cout << "Y0 coordinate: " << stoi(tokens[4]) << endl;
            // cout << "X1 coordinate: " << stoi(tokens[7]) << endl;
            // cout << "Y1 coordinate: " << stoi(tokens[8]) << endl;

            //// Example output
            // cout << "Region name: " << allRegions[number].getName() << endl;
            // cout << "X0 coordinate: " << allRegions[number].getX0() << endl;
            // cout << "Y0 coordinate: " << allRegions[number].getY0() << endl;
            // cout << "X1 coordinate: " << allRegions[number].getX1() << endl;
            // cout << "Y1 coordinate: " << allRegions[number].getY1() << endl;
        }
        if(tokens[0] == "END" && tokens[1] == "DESIGN")
        {
            break;
        }
    }
    //cout << "Unit distance in microns: " << unit_distance_microns << endl;
    //cout << "Block num: " << block_num << endl;
    //cout << "Region num: " << region_num << endl;

    fin.close();
}

void InputDataParse::parseBlockData(int block_num) 
{
    //cout << "block_num: " << block_num << endl;
    for (int i = 0; i < block_num; ++i) 
    {
        string filename = "blk_" + to_string(i) + ".def";
        ifstream fin(base_dir + "/" + case_name + "/" + case_name + "_def/" + filename);
        //cout << "Reading " << filename << endl;

        // check if the file is open
        if (!fin.is_open()) 
        {
            cout << "-- " << filename << " does not exist--" << endl;
            string block_id_name = allBlocks[i].getBlockIdName();

            // Extract the number part after the underscore
            size_t pos = block_id_name.find('_');
            string number_str = block_id_name.substr(pos + 1);
            int number = stoi(number_str);

            // Access the block_id_name to get the block definition file and update the block i
            for (size_t j = 0; j < allBlocks[number].getVertics().size(); j++) 
            {
                Blk &block = allBlocks[i];
                block.addVertics(allBlocks[number].getVertics()[j].first, allBlocks[number].getVertics()[j].second);
            }
        }
        else
        {
            vector<string> tokens;
            string line;
            int state = 0;

            while (getline(fin, line) && state == 0) 
            {
                //cout << line << endl;
                tokens = split(line, ' '); // Assuming split function correctly tokenizes the line
                if (tokens.empty())
                    continue;
                else if (tokens[0] == "DIEAREA") 
                {
                    //cout << "test" << endl;
                    // Extract the vertices of the DIEAREA
                    for (size_t j = 2; j < tokens.size(); j += 4) 
                    {
                        int x = stoi(tokens[j]); // Removing '(' from the string
                        int y = stoi(tokens[j + 1]);
                        // Store the vertices in a vector
                        Blk &block = allBlocks[i];
                        block.addVertics(x, y);
                        //cout << "X: " << x << " Y: " << y << endl;
                    }

                    // Handle the extracted DIEAREA data
                    // For example, you can print it or store it in a data structure
                    // for (size_t j = 0; j < allBlocks[i].getVertics().size(); j++) 
                    // {
                    //     cout << "X: " << allBlocks[i].getVertics()[j].first << " Y: " << allBlocks[i].getVertics()[j].second << endl;
                    // }
                }
                else if (tokens[0] == "END") 
                {
                    state = 1;
                }
            }
            fin.close();
            //cout << "Block " << i << " has been parsed" << endl;
            }
    }

    ifstream fin(base_dir + "/" + case_name + "/" + case_name + "_cfg.json");
    //cout << "Reading " << case_name << "_cfg.json" << endl;

    if (!fin.is_open()) {
        cerr << "Unable to open file: " << case_name + "_cfg.json" << endl;
        return;
    }

    nlohmann :: json j;
    fin >> j;

    for (const auto &block : j) {
        string block_name = block["block_name"].get<string>();
        int id = stoi(block_name.substr(6)); // Assuming block_name is in the format "BLOCK_x"
        Blk &b = allBlocks[id];

        cout << "Block name: " << block_name << endl;
        //b.setThroughBlockNetNum(block["through_block_net_num"].get<int>());

        for (const auto &edge_net : block["through_block_edge_net_num"]) {
            int x0 = edge_net[0][0];
            int y0 = edge_net[0][1];
            int x1 = edge_net[1][0];
            int y1 = edge_net[1][1];
            int value = edge_net[2];
            b.addThroughBlockEdgeNetNum(x0, y0, x1, y1, value);
            //cout << "Edge net: (" << x0 << ", " << y0 << ") -> (" << x1 << ", " << y1 << ") Value: " << value << endl;
        }

        for (const auto &port_region : block["block_port_region"]) {
            int x0 = port_region[0];
            int y0 = port_region[1];
            int x1 = port_region[2];
            int y1 = port_region[3];
            b.addBlockPortRegion(x0, y0, x1, y1);
            //cout << "Port region: (" << x0 << ", " << y0 << ") -> (" << x1 << ", " << y1 << ")" << endl;
        }

        b.setFeedthroughable(block["is_feedthroughable"].get<string>() == "True");
        b.setIsTile(block["is_tile"].get<string>() == "True");

        // cout << "Through block net num: " << b.getThroughBlockNetNum() << endl;
        // cout << "Feedthroughable: " << b.isFeedthroughable() << endl;
    }

    fin.close();

    //example output
    for (int i = 0; i < block_num; i++) {
        cout << "-----------------------------------" << endl;
        cout << "Block name: " << allBlocks[i].getBlockName() << endl;
        cout << "X coordinate: " << allBlocks[i].getLocX() << endl;
        cout << "Y coordinate: " << allBlocks[i].getLocY() << endl;
        cout << "Direction: " << allBlocks[i].getDirection() << endl;
        cout << "Through block net num: " << allBlocks[i].getThroughBlockNetNum() << endl;
        cout << "Feedthroughable: " << allBlocks[i].isFeedthroughable() << endl;
        cout << "Is tile: " << allBlocks[i].isTile() << endl;

        for (const auto &edge_net : allBlocks[i].getThroughBlockEdgeNetNum()) {
            cout << "Edge net: (" << edge_net.first.first << ", " << edge_net.first.second << ") -> (" << edge_net.second << ")" << endl;
        }

        for (const auto &port_region : allBlocks[i].getBlockPortRegion()) {
            cout << "Port region: (" << port_region.first << ", " << port_region.second << ") -> (" << port_region.first << ", " << port_region.second << ")" << endl;
        }

        for (const auto &vertex : allBlocks[i].getVertics()) {
            cout << "Vertex: (" << vertex.first << ", " << vertex.second << ")" << endl;
        }
    }

}

void InputDataParse::parseNetData(const string &case_name) 
{
    // read chip_top.def from file
    ifstream fin(base_dir + "/" + case_name + "/" + case_name + ".json");
    cout << "Reading " + case_name + ".json" << endl;
    
    if (!fin.is_open()) {
        cerr << "Unable to open file: " << case_name + ".json" << endl;
        return;
    }

    nlohmann :: json j;
    fin >> j;

    cout << "Parsing net data" << endl;

    // get the net number
    // 使用 JSON 對象的大小來計算網路的數量
    net_num = j.size();

    // build a vector of Net objects
    for (int i = 0; i < net_num; i++) 
    {
        allNets.push_back(Net(i));
    }
    
    //cout << "Net num: " << max_id << endl;

    for (const auto &net : j)
    {
        int id = net["ID"].get<int>();
        Net &n = allNets[id];

        //cout << "ID" << endl;
        //RX
        for (const auto &rx : net["RX"]) {
            //cout << "RX: " << rx.get<string>() << endl;
            n.addRx(rx.get<string>());
            //cout << "RX: " << n.getRx().back() << endl;
        }

        cout << "TX" << endl;
        //TX
        for (const auto &tx : net["TX"]) {
            //cout << "TX: " << tx.get<string>() << endl;
            n.addTx(tx.get<string>());
            //cout << "TX: " << n.getTx().back() << endl;
        }

        //NUM
        //cout << "NUM: " << net["NUM"].get<int>() << endl;
        n.setNum(net["NUM"].get<int>());
        cout << "NUM: " << n.getNum() << endl;

        cout << "MUST_THROUGH" << endl;
        //must_through “MUST_THROUGH”: [[D,(X0,Y0,X1,Y1),(X2,Y2,X3,Y3)], 
        for (const auto &must_through : net["MUST_THROUGH"]) {
            cout << "test" << endl;
            string key = must_through[0];

            int x0 = must_through[1][0].get<int>();
            int y0 = must_through[1][1].get<int>();
            int x1 = must_through[1][2].get<int>();
            int y1 = must_through[1][3].get<int>();
            int x2 = must_through[2][0].get<int>();
            int y2 = must_through[2][1].get<int>();
            int x3 = must_through[2][2].get<int>();
            int y3 = must_through[2][3].get<int>();

            // cout << "key: " << key << endl;
            // cout << "x0: " << x0 << endl;
            // cout << "y0: " << y0 << endl;
            // cout << "x1: " << x1 << endl;
            // cout << "y1: " << y1 << endl;
            // cout << "x2: " << x2 << endl;
            // cout << "y2: " << y2 << endl;
            // cout << "x3: " << x3 << endl;
            // cout << "y3: " << y3 << endl;

            n.addMustThrough(key, x0, y0, x1, y1, x2, y2, x3, y3);

            // cout << "key: " << n.getMustThrough().begin()->first << endl;
            // cout << "x0: " << n.getMustThrough().begin()->second[0] << endl;
            // cout << "y0: " << n.getMustThrough().begin()->second[1] << endl;
            // cout << "x1: " << n.getMustThrough().begin()->second[2] << endl;
            // cout << "y1: " << n.getMustThrough().begin()->second[3] << endl;
            // cout << "x2: " << n.getMustThrough().begin()->second[4] << endl;
            // cout << "y2: " << n.getMustThrough().begin()->second[5] << endl;
            // cout << "x3: " << n.getMustThrough().begin()->second[6] << endl;
            // cout << "y3: " << n.getMustThrough().begin()->second[7] << endl;
        }

        cout << "HMFT_MUST_THROUGH" << endl;
        //hmft_must_through “HMFT_MUST_THROUGH”: [[D,(X0,Y0,X1,Y1),(X2,Y2,X3,Y3)],
        for (const auto &hmft_must_through : net["HMFT_MUST_THROUGH"]) {
            string key = hmft_must_through[0];

            int x0 = hmft_must_through[1][0].get<int>();
            int y0 = hmft_must_through[1][1].get<int>();
            int x1 = hmft_must_through[1][2].get<int>();
            int y1 = hmft_must_through[1][3].get<int>();
            int x2 = hmft_must_through[2][0].get<int>();
            int y2 = hmft_must_through[2][1].get<int>();
            int x3 = hmft_must_through[2][2].get<int>();
            int y3 = hmft_must_through[2][3].get<int>();

            // cout << "key: " << key << endl;
            // cout << "x0: " << x0 << endl;
            // cout << "y0: " << y0 << endl;
            // cout << "x1: " << x1 << endl;
            // cout << "y1: " << y1 << endl;
            // cout << "x2: " << x2 << endl;
            // cout << "y2: " << y2 << endl;
            // cout << "x3: " << x3 << endl;
            // cout << "y3: " << y3 << endl;

            n.addHmftMustThrough(key, x0, y0, x1, y1, x2, y2, x3, y3);  

            // cout << "key: " << n.getHmftMustThrough().begin()->first << endl;
            // cout << "x0: " << n.getHmftMustThrough().begin()->second[0] << endl;
            // cout << "y0: " << n.getHmftMustThrough().begin()->second[1] << endl;
            // cout << "x1: " << n.getHmftMustThrough().begin()->second[2] << endl;
            // cout << "y1: " << n.getHmftMustThrough().begin()->second[3] << endl;
            // cout << "x2: " << n.getHmftMustThrough().begin()->second[4] << endl;
            // cout << "y2: " << n.getHmftMustThrough().begin()->second[5] << endl;
            // cout << "x3: " << n.getHmftMustThrough().begin()->second[6] << endl;
            // cout << "y3: " << n.getHmftMustThrough().begin()->second[7] << endl;   
        }

        // TX_COORD
        cout << "TX_COORD type: " << net.at("TX_COORD").type_name() << endl;
        double x = net.at("TX_COORD")[0].get<double>();
        double y = net.at("TX_COORD")[1].get<double>();
        n.addTxCoord(x, y);

        // RX_COORD
        for (const auto &rx_coord : net["RX_COORD"]) {
            int x = rx_coord[0].get<double>();
            int y = rx_coord[1].get<double>();
            n.addRxCoord(x, y);
        }
    }

    fin.close();
}

void InputDataParse::printBlockData() {
    cout << "Printing block data" << endl;
    for (int i = 0; i < block_num; i++) {
        cout << "-----------------------------------" << endl;
        cout << "Block name: " << allBlocks[i].getBlockName() << endl;
        cout << "X coordinate: " << allBlocks[i].getLocX() << endl;
        cout << "Y coordinate: " << allBlocks[i].getLocY() << endl;
        cout << "Direction: " << allBlocks[i].getDirection() << endl;
        cout << "Through block net num: " << allBlocks[i].getThroughBlockNetNum() << endl;
        cout << "Feedthroughable: " << allBlocks[i].isFeedthroughable() << endl;
        cout << "Is tile: " << allBlocks[i].isTile() << endl;

        for (const auto &edge_net : allBlocks[i].getThroughBlockEdgeNetNum()) {
            cout << "Edge net: (" << edge_net.first.first << ", " << edge_net.first.second << ") -> (" << edge_net.second << ")" << endl;
        }

        for (const auto &port_region : allBlocks[i].getBlockPortRegion()) {
            cout << "Port region: (" << port_region.first << ", " << port_region.second << ") -> (" << port_region.first << ", " << port_region.second << ")" << endl;
        }

        for (const auto &vertex : allBlocks[i].getVertics()) {
            cout << "Vertex: (" << vertex.first << ", " << vertex.second << ")" << endl;
        }

        cout << "-----------------------------------" << endl;
    }
}

void InputDataParse::printNetData() {
    cout << "Printing net data" << endl;
    cout << "Net num: " << net_num << endl;
    for (int i = 0; i < net_num; i++) {

        cout << "Net ID: " << allNets[i].getId() << endl;

        cout << "RX: ";
        for (const auto &rx : allNets[i].getRx()) {
            cout << rx << endl;
        }

        cout << "TX: ";
        for (const auto &tx : allNets[i].getTx()) {
            cout << tx << endl;
        }
        cout << "NUM: " << allNets[i].getNum() << endl;

        cout << "MUST_THROUGH: " << endl;
        for (const auto &must_through : allNets[i].getMustThrough()) {
            cout << must_through.first << ": (" << must_through.second[0] << ", " << must_through.second[1] << ") -> (" << must_through.second[2] << ", " << must_through.second[3] << ") -> (" << must_through.second[4] << ", " << must_through.second[5] << ") -> (" << must_through.second[6] << ", " << must_through.second[7] << ")" << endl;
        }

        cout << "HMFT_MUST_THROUGH: " << endl;
        for (const auto &hmft_must_through : allNets[i].getHmftMustThrough()) {
            cout << hmft_must_through.first << ": (" << hmft_must_through.second[0] << ", " << hmft_must_through.second[1] << ") -> (" << hmft_must_through.second[2] << ", " << hmft_must_through.second[3] << ") -> (" << hmft_must_through.second[4] << ", " << hmft_must_through.second[5] << ") -> (" << hmft_must_through.second[6] << ", " << hmft_must_through.second[7] << ")" << endl;
        }

        cout << "TX_COORD: " << endl;
        for (const auto &tx_coord : allNets[i].getTxCoord()) {
            cout << "(" << tx_coord.first << ", " << tx_coord.second << ")" << endl;
        }

        cout << "RX_COORD: " << endl;
        for (const auto &rx_coord : allNets[i].getRxCoord()) {
            cout << "(" << rx_coord.first << ", " << rx_coord.second << ")" << endl;
        }

        cout << "-----------------------------------" << endl;
    }
}

void InputDataParse::writeFile(const string &case_name)
{
    // Block data write to file
    ofstream fout("block.txt");
    if (!fout.is_open())
    {
        cerr << "Unable to open file: block_data.txt" << endl;
        return;
    }

    for (int i = 0; i < block_num; i++)
    {
        // calculate the coordinate of vertex of the block
        int x0 = allBlocks[i].getLocX();
        int y0 = allBlocks[i].getLocY();

        fout << allBlocks[i].getBlockName() << " ";

        for(size_t j = 0; j < allBlocks[i].getVertics().size(); j++)
        {
            int dx = allBlocks[i].getVertics()[j].first;
            int dy = allBlocks[i].getVertics()[j].second;
            fout << "(" << x0 + dx << "," << y0 + dy << ") ";
        }
        fout << DirectionToString(allBlocks[i].getDirection()) << endl;
    }

    fout.close();

    // Region data write to file
    ofstream fout1("region.txt");
    if (!fout1.is_open())
    {
        cerr << "Unable to open file: region_data.txt" << endl;
        return;
    }

    for (int i = 0; i < region_num; i++)
    {
        fout1 << allRegions[i].getName() << " ";
        fout1 << "(" << allRegions[i].getX0() << "," << allRegions[i].getY0() << ") ";
        fout1 << "(" << allRegions[i].getX1() << "," << allRegions[i].getY1() << ")" << endl;
    }


    
}

Direction stringToDirection(const string &str)
    {
        if (str == "N") return N;
        if (str == "E") return E;
        if (str == "S") return S;
        if (str == "W") return W;
        if (str == "FN") return FN;
        if (str == "FE") return FE;
        if (str == "FS") return FS;
        if (str == "FW") return FW;
        throw invalid_argument("Invalid direction string: " + str);
    }

//DirectionTostring
string DirectionToString(Direction dir)
{
    switch (dir)
    {
    case N:
        return "N";
    case E:
        return "E";
    case S:
        return "S";
    case W:
        return "W";
    case FN:
        return "FN";
    case FE:
        return "FE";
    case FS:
        return "FS";
    case FW:
        return "FW";
    default:
        throw invalid_argument("Invalid direction");
    }
}
#endif