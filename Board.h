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
        round = 0;
    }

    Board(int _round, string _player_name, Table _table, vector<Player> _players) {
        round = _round;
        player_name = _player_name;
        table = _table;
        players = _players;
    }

    Board(Json::Value value) {
        round = value["round"].asInt();
        player_name = value["playerName"].asString();
//        cout << player_name << endl;
        table = Table(value["table"]);
//        cout << table.cards.size() << endl;
        Json::Value playersVal = value["players"];
        for (int i = 0; i < playersVal.size(); i++) {
            players.emplace_back(playersVal[i]);
        }
    }

    Json::Value toJson() {
        Json::Value value;
        value["round"] = round;
        value["playerName"] = player_name;
        value["table"] = table.toJson();
        Json::Value playersVal = Json::arrayValue;
        for (int i = 0; i < players.size(); i++) {
            playersVal.append(players[i].toJson());
        }
        value["players"] = playersVal;
        return value;
    }

    void execute(const Move& move) {
        bool f;
        bool found_card;
        bool money_enough;
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
                        if (table.gems.gems["gold"] > 0) {
                            players[i].gems.gems["gold"]++;
                            table.gems.gems["gold"]--;
                        }
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
                break;
            case Move::Type::BUY_RES:
                f = false;
                for (int i = 0; i < players.size(); i++) {
                    if (players[i].name == move.player) {
                        f = true;
                        found_card = false;
                        for (int j = 0; j < players[i].reserved_cards.size(); j++) {
                            if (players[i].reserved_cards[j] == move.card) {
                                found_card = true;
                                if (players[i].canBuyCard(move.card)) {
                                    players[i].buyCard(move.card);
                                    players[i].reserved_cards.erase(players[i].reserved_cards.begin() + j);
                                    break;
                                }
                                else
                                    cout << "can't afford card" << endl;
                                break;
                            }
                        }
                        if (!found_card) cout << "can't find card" << endl;
                        break;
                    }
                }
                if (!f) cout << "Can't find player" << endl;
                break;
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
