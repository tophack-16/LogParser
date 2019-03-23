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
    string color_str;
    GemCluster color;
    GemCluster costs;

    NormalCard() {

    }

    NormalCard(const Json::Value& value) {
        level = value["level"].asInt();
        score = 0;
        if (value["score"].isInt())
            score = value["score"].asInt();
        color_str = value["color"].asString();
        color = GemCluster(color_str);
        Json::Value costsVal = value["costs"];
        costs = GemCluster(costsVal);
    }

    Json::Value toJson() {
        Json::Value value;
        value["level"] = level;
        if (score > 0)
            value["score"] = score;
        value["color"] = color_str;
        value["costs"] = costs.toJson();
//        cout << "Card: " << value.size() << endl;
        return value;
    }

    string toString() {
        string s;
        s += "NormalCard\n";
        s += "level=" + to_string(level) + ", score=" + to_string(score) + '\n';
        s += "color=" + color_str + '\n';
        s += "costs=" + costs.toString();
        return s;
    }

    friend bool operator == (const NormalCard& a, const NormalCard& b) {
        return a.level == b.level && a.score == b.score && a.color == b.color && a.costs == b.costs;
    }
};

class NobelCard {
public:
    int score;
    GemCluster requirements;

    NobelCard() {

    }

    NobelCard(const Json::Value& value) {
        score = 0;
        if (value["score"].isInt())
            score = value["score"].asInt();
        requirements = GemCluster(value["requirements"]);
    }

    Json::Value toJson() {
        Json::Value value;
        if (score > 0)
            value["score"] = score;
        value["requirements"] = requirements.toJson();
        return value;
    }

    string toString() {
            string s;
            s += "NobelCard\n";
            s += "score=" + to_string(score) + '\n';
            s += "requirements=" + requirements.toString();
            return s;
    }

    friend bool operator == (const NobelCard& a, const NobelCard& b) {
        return a.score == b.score && a.requirements == b.requirements;
    }
};


#endif //PLAYER_CARD_H
