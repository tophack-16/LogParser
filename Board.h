//
// Created by zhanghuimeng on 19-3-23.
//

#ifndef PLAYER_BOARD_H
#define PLAYER_BOARD_H

#include <string>
#include <vector>
#include "Table.h"
#include "Player.h"

using namespace std;

class Board {
private:
    int round;
    string player_name;
    Table table;
    vector<Player> players;

public:
    Board(int _round, string _player_name, Table _table, vector<Player> _players) {
        round = _round;
        player_name = _player_name;
        table = _table;
        players = _players;
    }
};


#endif //PLAYER_BOARD_H
