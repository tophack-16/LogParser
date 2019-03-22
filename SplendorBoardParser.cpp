//
// Created by zhanghuimeng on 19-3-23.
//

#include "Board.h"
#include "json/json.h"

#define TEST

#ifdef TEST
#include <iostream>
#endif

static Board boardParser(const string& input) {
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(input, value)) {
        int round = value["round"].asInt();
        string player_name = value["playerName"].asString();
#ifdef TEST
        cout << "Read round=" << round << endl;
        cout << "Read player_name=" << player_name << endl;
#endif
        Board board(round, player_name);
        return board;
    }
    else
        exit(0);
}
