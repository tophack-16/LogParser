#include <iostream>
#include <fstream>
#include <string>
#include <argp.h>
#include "stdbool.h"
#include <assert.h>
#include "SplendorBoardParser.cpp"
#include "SplendorLogParser.cpp"
#include "cerrLogParser.cpp"
#include "frontend.cpp"
#include "Frontend.h"

using namespace std;

int main()
{
    string input, line;
    string filename = "output/board_round_1_turn_1.json";
//    string filename = "sample_input.json";
    cout << "Tesing Board" << endl;
    ifstream fin(filename);
    while (getline(fin, line)) input += line;
    Json::Reader reader;
    Json::Value value;
    reader.parse(input, value);
    Board board(value);
    cout << "player=" << board.player_name << endl;
    cout << "gems=" << board.table.gems.toString() << endl;
    cout << "==== End of Test ====" << endl;
//    ifstream log_file("stderr_hhh.txt");
//    vector<string> lines;
//    while (getline(log_file, line)) {
//        lines.push_back(line);
//    }
//    cerrLogParser(lines);

//    frontend();

    Frontend f;
    f.work();
    return 0;
}
