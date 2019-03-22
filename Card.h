//
// Created by zhanghuimeng on 19-3-22.
//

#ifndef PLAYER_CARD_H
#define PLAYER_CARD_H

#include <string>
#include "GemCluster.h"
#include "json/json.h"

using namespace std;

class NormalCard {
public:
    int level;
    int score;
    GemCluster color;
    GemCluster costs;

    NormalCard(const Json::Value& value) {
        level = value["level"].asInt();
        score = value["score"].asInt();
        color = GemCluster(value["color"].asString());
        Json::Value costsVal = value["costs"];
        costs = GemCluster(costsVal);
    }

    string toString() {
        string s;
        s += "NormalCard\n";
        s += "level=" + to_string(level) + ", score=" + to_string(score) + '\n';
        s += "color=" + color.toString() + '\n';
        s += "costs=" + costs.toString();
        return s;
    }
};

class NobelCard {
public:
    int score;
    GemCluster requirements;

    NobelCard(const Json::Value& value) {
        score = value["score"].asInt();
        requirements = GemCluster(value["requirements"]);
    }

    string toString() {
            string s;
            s += "NobelCard\n";
            s += "score=" + to_string(score) + '\n';
            s += "requirements=" + requirements.toString();
            return s;
    }
};


#endif //PLAYER_CARD_H
