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

public:
    InputDataParse(const string &base_dir, const string &case_name) : base_dir(base_dir), case_name(case_name) {}

    void parse()
    {
        // TODO: parse chip_top.def
        parseChipTop();
        // TODO: parse block data
        parseBlockData(block_num);
        // TODO: parse caseX_cfg.json
        // TODO: parse net data(caseX.json)
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
        tokens = split(line, ' ');
        if (tokens[0] == "UNITS")
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
            string orientation = tokens[9]; // Assuming "N" is the orientation in this example

            Blk(number).setLocX(x_coord);
            Blk(number).setLocY(y_coord);
            Blk(number).setDirection(N);
            Blk(number).setBlockName(blk_name);

            // Store the block in a vector
            allBlocks.push_back(Blk(number));

            // Example output
            cout << "Block name: " << blk_name << endl;
            cout << "X coordinate: " << x_coord << endl;
            cout << "Y coordinate: " << y_coord << endl;
            cout << "Orientation: " << orientation << endl;
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

            Region(number).setX0(stoi(tokens[3]));
            Region(number).setY0(stoi(tokens[4]));
            Region(number).setX1(stoi(tokens[7]));
            Region(number).setY1(stoi(tokens[8]));

            // Store the region in a vector
            allRegions.push_back(Region(number));

            // Example output
            cout << "Region name: " << region_name << endl;
            cout << "X0 coordinate: " << stoi(tokens[3]) << endl;
            cout << "Y0 coordinate: " << stoi(tokens[4]) << endl;
            cout << "X1 coordinate: " << stoi(tokens[7]) << endl;
        }
    }
    cout << "Unit distance in microns: " << unit_distance_microns << endl;
    cout << "Block num: " << block_num << endl;
    cout << "Region num: " << region_num << endl;
}

void InputDataParse::parseBlockData(int block_num) {
    int i = 0;
    while (i < block_num) {
        string filename = "blk_" + to_string(i) + ".def";
        ifstream fin(base_dir + "/" + case_name + "/" + case_name + "_def/" + filename);
        cout << "Reading " << filename << endl;
        vector<string> tokens;
        string line;

        while (getline(fin, line)) {
            tokens = split(line, ' ');
            
            if (tokens[0] == "DIEAREA") {
                for (size_t j = 2; j < tokens.size(); j += 3) {
                    int x = stoi(tokens[j]); // Removing '(' from the string
                    int y = stoi(tokens[j+1]);
                    allBlocks[i].addVertics(x, y);
                }

                // Handle the extracted DIEAREA data
                // For example, you can print it or store it in a data structure
                for (size_t j = 0; j < allBlocks[i].getVertics().size(); j++) {
                    cout << "X: " << allBlocks[i].getVertics()[j].first << " Y: " << allBlocks[i].getVertics()[j].second << endl;
                }
            }
        }
        i++;
    }

    ifstream fin(base_dir + "/" + case_name + "/" + case_name + "_cfg.json");
    cout << "Reading " << case_name << "_cfg.json" << endl;

    if (!fin.is_open()) {
        cerr << "Unable to open file: " << case_name + "_cfg.json" << endl;
        return;
    }

    nlohmann :: json j;
    fin >> j;

    for (const auto &block : j) {
        string block_name = block["block_name"].get<string>();
        int id = stoi(block_name.substr(6)); // Assuming block_name is in the format "BLOCK_x"
        Blk blk(id);

        allBlocks[id].setBlockName(block_name);
        allBlocks[id].setThroughBlockNetNum(block["through_block_net_num"].get<int>());

        for (const auto &edge_net : block["through_block_edge_net_num"]) {
            int x0 = edge_net[0][0];
            int y0 = edge_net[0][1];
            int x1 = edge_net[1][0];
            int y1 = edge_net[1][1];
            int value = edge_net[2];
            allBlocks[id].addThroughBlockEdgeNetNum(x0, y0, x1, y1, value);
        }

        for (const auto &port_region : block["block_port_region"]) {
            int x0 = port_region[0];
            int y0 = port_region[1];
            int x1 = port_region[2];
            int y1 = port_region[3];
            allBlocks[id].addBlockPortRegion(x0, y0, x1, y1);
        }

        allBlocks[id].setFeedthroughable(block["is_feedthroughable"].get<string>() == "True");
        allBlocks[id].setIsTile(block["is_tile"].get<string>() == "True");
    }
}
#endif