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
        // Table
        Table table(value["table"]);
        // Player
        Json::Value playersVal = value["players"];
        vector<Player> players;
        for (int i = 0; i < playersVal.size(); i++) {
            players.emplace_back(playersVal[i]);
        }
        Board board(round, player_name, table, players);
        return board;
    }
    else
        exit(0);
}
