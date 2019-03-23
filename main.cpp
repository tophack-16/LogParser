#include <iostream>
#include <fstream>
#include <string>
#include <argp.h>
#include "stdbool.h"
#include <assert.h>
#include "SplendorBoardParser.cpp"
#include "SplendorLogParser.cpp"

using namespace std;

int main()
{
    string input, line;
    string filename = "output/board_round_4_turn_2.json";
//    string filename = "sample_input.json";
    cout << "Tesing Board" << endl;
    // TODO：输出用下划线
    ifstream fin(filename);
    while (getline(fin, line)) input += line;
    Json::Reader reader;
    Json::Value value;
    reader.parse(input, value);
    Board board(value);
    cout << "player=" << board.player_name << endl;
    cout << "gems=" << board.table.gems.toString() << endl;
    cout << "==== End of Test ====" << endl;
    ifstream log_file("log_hhh.txt");
    vector<string> lines;
    while (getline(log_file, line)) lines.push_back(line);
    logParser(lines);
    return 0;
}
