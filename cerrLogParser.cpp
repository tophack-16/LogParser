//
// Created by zhanghuimeng on 19-3-24.
//

#include <string>
#include <fstream>
#include "json/json.h"
#include <vector>
#include "Board.h"

using namespace std;

static void cerrLogParser(const vector<string>& log) {
    int round;
    int turn = 0;
    Json::Reader reader;
    Json::Value value;
    string json;
    vector<Board> boards;
    vector<Move> moves;
    Json::StyledStreamWriter writer;
    for (const string& line: log) {
        if (line.empty()) {
            if (json.empty()) continue;
            else {
                reader.parse(json, value);
                if (!value["input"].isNull()) {
                    Board board(value["input"]);
                    round = board.round;
                    cout << "Board: " << round << ' ' << turn << endl;
                    ofstream fout("output/board_round_" + to_string(round) +
                                  "_turn_" + to_string(turn + 1) + ".json");
                    writer.write(fout, board.toJson());
                    fout.close();
                    turn = (turn + 1) % 3;
                }
                else if (!value["output"].isNull()) {
                    ofstream fout("output/action_round_" + to_string(round) +
                                  "_turn_" + to_string(turn + 1) + ".json");
                    writer.write(fout, value["output"]);
                    fout.close();
                }
                json.clear();
            }
        }
        else
            json += line;
    }
}