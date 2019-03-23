//
// Created by zhanghuimeng on 19-3-22.
// 可以比较和加减的Gem集合，可以用于表示红利、花费、场上的gem数量和持有gem数量。
//

#ifndef PLAYER_GEMCLUSTER_H
#define PLAYER_GEMCLUSTER_H

#include <string>
#include <map>
#include <vector>
#include "json/json.h"

using namespace std;

class GemCluster {
public:
    map<string, int> gems;
    static const vector<string> colors;

    GemCluster() {
        for (string color: colors)
            gems[color] = 0;
    }

    GemCluster(vector<string> v):GemCluster() {
        for (string color: v)
            gems.at(color)++;
    }

    GemCluster(string color):GemCluster() {
        gems[color]++;
    }

    GemCluster(const Json::Value& list):GemCluster() {
        for (int i = 0; i < list.size(); i++)
        {
            gems[list[i]["color"].asString()] = list[i]["count"].asInt();
        }
    }

    Json::Value toJson() {
        Json::Value value = Json::arrayValue;
        for (string color: colors)
            if (gems[color] > 0) {
                Json::Value gemVal;
                gemVal["color"] = color;
                gemVal["count"] = gems[color];
                value.append(gemVal);
            }
        return value;
    }

    friend bool operator == (const GemCluster& a, const GemCluster& b) {
        for (string color: colors) {
            if (a.gems.at(color) != b.gems.at(color)) return false;
        }
        return true;
    }

    friend bool operator >= (const GemCluster& a, const GemCluster& b) {
        // TODO：这是特殊情况，b的gold必为0
        int gold_num = a.gems.at("gold");
        for (string color: colors) {
            if (color != "gold") {
                if (a.gems.at(color) < b.gems.at(color)) {
                    if (b.gems.at(color) - a.gems.at(color) <= gold_num)
                        gold_num -= b.gems.at(color) - a.gems.at(color);
                    else return false;
                }
            }
        }
        return true;
    }

    friend GemCluster operator + (const GemCluster& a, const GemCluster& b) {
        GemCluster c;
        for (string color: colors) {
            c.gems[color] = a.gems.at(color) + b.gems.at(color);
        }
        return c;
    }

    void operator += (const GemCluster& a) {
        for (string color: colors) {
            gems[color] = gems[color] + a.gems.at(color);
        }
    }

    friend GemCluster operator - (const GemCluster& a, const GemCluster& b) {
        // TODO：这是特殊情况，b的gold必为0
        GemCluster c;
        int gold_num = a.gems.at("gold");
        for (string color: colors) {
            if (color != "gold") {
                if (a.gems.at(color) < b.gems.at(color)) {
                    gold_num -= b.gems.at(color) - a.gems.at(color);
                }
                else
                    c.gems[color] = a.gems.at(color) - b.gems.at(color);
            }
            c.gems["gold"] = gold_num;
        }
        return c;
    }

    string toString() {
        string s;
        for (string color: colors)
            s += color.substr(0, 1) + "=" + to_string(gems[color]) + ", ";
        return s;
    }
};


#endif //PLAYER_GEMCLUSTER_H
