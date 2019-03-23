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
//        cout << name << endl;
        if (value["score"].isInt()) {
            score = value["score"].asInt();
        }
        if (value["gems"].isArray())
            gems = GemCluster(value["gems"]);
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

    Json::Value toJson() {
        Json::Value value;
        value["name"] = name;
        value["score"] = score;
        value["gems"] = gems.toJson();
        if (purchased_cards.size() > 0) {
            Json::Value purchasedVal = Json::arrayValue;
            for (int i = 0; i < purchased_cards.size(); i++)
                purchasedVal.append(purchased_cards[i].toJson());
            value["purchasedCards"] = purchasedVal;
        }
        if (reserved_cards.size() > 0) {
            Json::Value reservedVal = Json::arrayValue;
            for (int i = 0; i < reserved_cards.size(); i++)
                reservedVal.append(reserved_cards[i].toJson());
            value["reservedCards"] = reservedVal;
        }
        return value;
    }

    bool canBuyCard(NormalCard card) {
        // 减去红利
        for (int i = 0; i < purchased_cards.size(); i++) {
            if (card.costs.gems[purchased_cards[i].color_str] > 0)
                card.costs.gems[purchased_cards[i].color_str]--;
        }
        return gems >= card.costs;
    }

    void buyCard(NormalCard card) {
        // 减去红利
        for (int i = 0; i < purchased_cards.size(); i++) {
            if (card.costs.gems[purchased_cards[i].color_str] > 0)
                card.costs.gems[purchased_cards[i].color_str]--;
        }
        gems = gems - card.costs;
        score += card.score;
    }
};


#endif //PLAYER_USER_H
