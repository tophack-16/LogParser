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
                if (!table.reserve(move.card)) {
                    cout << "Can't find card" << endl;
                    break;
                }
                // TODO：假定这三个名字有不同……
                this->getPlayerByName(move.player).reserved_cards.push_back(move.card);
                if (table.gems.gems["gold"] > 0) {
                    this->getPlayerByName(move.player).gems.gems["gold"]++;
                    table.gems.gems["gold"]--;
                }
                break;
            case Move::Type::RES2:
                // TODO：假定这三个名字有不同……
                this->getPlayerByName(move.player).reserved_cards.push_back(move.card);
                if (table.gems.gems["gold"] > 0) {
                    this->getPlayerByName(move.player).gems.gems["gold"]++;
                    table.gems.gems["gold"]--;
                }
                break;
            case Move::Type::DIFF_COL:
                this->getPlayerByName(move.player).gems += move.acqGem;
                table.gems = table.gems - move.acqGem;
                break;
            case Move::Type::BUY_RES:
                getPlayerByName(move.player).deleteReservedCard(move.card);
                getPlayerByName(move.player).purchased_cards.push_back(move.card);
                table.gems += getPlayerByName(move.player).buyCard(move.card);
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

    Player& getPlayerByName(const string& name) {
        for (int i = 0; i < players.size(); i++) {
            if (players[i].name == name)
                return players[i];
        }
        cout << "can't find this player on board" << endl;
    }

    GemCluster calcGemSum() {
        GemCluster sum = table.gems;
        for (int i = 0; i < players.size(); i++)
            sum += players[i].gems;
        return sum;
    }
};


#endif //PLAYER_BOARD_H
