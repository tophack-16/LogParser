//
// Created by zhanghuimeng on 19-3-22.
//

#ifndef PLAYER_USER_H
#define PLAYER_USER_H

#include "GemCluster.h"
#include "Card.h"

using namespace std;

class Player {
public:
    string name;
    int score;
    GemCluster gems;
    vector<NormalCard> purchased_cards;
    vector<NormalCard> reserved_cards;
    vector<NobelCard> nobles;

    Player(string _name = "") {
        name = _name;
        score = 0;
    }

    Player(const Json::Value& value): Player() {
        name = value["name"].asString();
        if (value["score"].isInt())
            score = value.asInt();
        if (value["gems"].isArray())
            gems = GemCluster(value["gems"]);
        // TODO：确定此位置是否为驼峰输入
        if (value["purchasedCards"].isArray()) {
            for (int i = 0; i < value["purchasedCards"].size(); i++)
                purchased_cards.emplace_back(value["purchasedCards"][i]);
        }
        if (value["reservedCards"].isArray()) {
            for (int i = 0; i < value["reservedCards"].size(); i++)
                reserved_cards.emplace_back(value["reservedCards"][i]);
        }
        if (value["nobles"].isArray()) {
            for (int i = 0; i < value["nobles"].size(); i++)
                nobles.emplace_back(value["nobles"][i]);
        }
    }
};


#endif //PLAYER_USER_H
