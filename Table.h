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
    vector<NobelCard> nobles;

    Table() {

    }

    Table(Json::Value value) {
//        for (auto iter = value.begin(); iter != value.end(); iter++) {
//            cout << "Table: " << iter.key() << endl;
//        }
        Json::Value gemsVal = value["gems"], cardsVal = value["cards"], noblesVal = value["nobles"];
        gems = GemCluster(gemsVal);
        for (int i = 0; i < cardsVal.size(); i++) {
            cards.emplace_back(cardsVal[i]);
        }
        for (int i = 0; i < noblesVal.size(); i++) {
            nobles.emplace_back(noblesVal[i]);
        }
    }

    bool reserve(const NormalCard& card) {
        for (int i = 0; i < cards.size(); i++) {
            if (cards[i] == card) {
                cards.erase(cards.begin() + i);
                return true;
            }
        }
        return false;
    }

    Json::Value toJson() {
        Json::Value value;
        value["gems"] = gems.toJson();
        Json::Value cardsVal = Json::arrayValue;
//        cout << "Table: " << cards.size() << endl;
        for (int i = 0; i < cards.size(); i++)
            cardsVal.append(cards[i].toJson());
        value["cards"] = cardsVal;
//        cout << cardsVal.size() << endl;
        Json::Value noblesVal = Json::arrayValue;
        for (int i = 0; i < nobles.size(); i++)
            noblesVal.append(nobles[i].toJson());
        value["nobles"] = noblesVal;
        return value;
    }
};


#endif //PLAYER_TABLE_H
