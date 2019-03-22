//
// Created by zhanghuimeng on 19-3-23.
//

#ifndef PLAYER_TABLE_H
#define PLAYER_TABLE_H

#include "GemCluster.h"
#include "Card.h"
#include "Move.h"

class Table {
public:
    GemCluster gems;
    vector<NormalCard> cards;
    vector<NobelCard> nobels;

    Table() {

    }

    Table(Json::Value value) {
        Json::Value gemsVal = value["gems"], cardsVal = value["cards"], noblesVal = value["nobles"];
        gems = GemCluster(gemsVal);
        for (int i = 0; i < cardsVal.size(); i++) {
            cards.emplace_back(cardsVal[i]);
        }
        for (int i = 0; i < noblesVal.size(); i++) {
            nobels.emplace_back(noblesVal[i]);
        }
    }
};


#endif //PLAYER_TABLE_H
