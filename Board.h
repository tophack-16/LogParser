//
// Created by zhanghuimeng on 19-3-23.
//

#ifndef PLAYER_BOARD_H
#define PLAYER_BOARD_H

#include <string>
#include <vector>
#include "Table.h"
#include "Player.h"
#include "Move.h"

using namespace std;

class Board {
public:
    int round;
    string player_name;
    Table table;
    vector<Player> players;

    Board() {
    }

    Board(int _round, string _player_name, Table _table, vector<Player> _players) {
        round = _round;
        player_name = _player_name;
        table = _table;
        players = _players;
    }

    void execute(const Move& move) {
        bool f;
        switch (move.type) {
            case Move::Type::RES1:
                f = false;
                for (int i = 0; i < table.cards.size(); i++) {
//                    cout << table.cards[i].toString() << endl;
                    if (table.cards[i] == move.card) {
                        f = true;
                        table.cards.erase(table.cards.begin() + i);
                        break;
                    }
                }
                if (!f) cout << "Can't find card" << endl;
                f = false;
                // TODO：假定这三个名字有不同……
                for (int i = 0; i < players.size(); i++) {
                    if (players[i].name == move.player) {
                        players[i].reserved_cards.push_back(move.card);
                        f = true;
                        break;
                    }
                }
                if (!f) cout << "Can't find player" << endl;
                break;
            case Move::Type::DIFF_COL:
                f = false;
                for (int i = 0; i < players.size(); i++) {
                    if (players[i].name == move.player) {
                        players[i].gems += move.acqGem;
                        f = true;
                        break;
                    }
                }
                if (!f) cout << "Can't find player" << endl;
            default:
                break;
        }
    }

    GemCluster getPlayerGembyName(const string& name) {
        for (int i = 0; i < players.size(); i++) {
            if (players[i].name == name)
                return players[i].gems;
        }
        cout << "can't find this player on board" << endl;
    }
};


#endif //PLAYER_BOARD_H
