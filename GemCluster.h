//
// Created by zhanghuimeng on 19-3-22.
// 可以比较和加减的Gem集合，可以用于表示红利、花费、场上的gem数量和持有gem数量。
//

#ifndef PLAYER_GEMCLUSTER_H
#define PLAYER_GEMCLUSTER_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class GemCluster {
private:
    map<string, int> gems;
    static const vector<string> colors;

public:
    GemCluster() {
        for (string color: colors)
            gems[color] = 0;
    }

    friend bool operator <= (const GemCluster& a, const GemCluster& b) {
        for (string color: colors) {
            if (a.gems.at(color) > b.gems.at(color)) return false;
        }
        return true;
    }

    friend bool operator >= (const GemCluster& a, const GemCluster& b) {
        for (string color: colors) {
            if (a.gems.at(color) < b.gems.at(color)) return false;
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

    friend GemCluster operator - (const GemCluster& a, const GemCluster& b) {
        GemCluster c;
        for (string color: colors) {
            c.gems[color] = a.gems.at(color) - b.gems.at(color);
        }
        return c;
    }
};


#endif //PLAYER_GEMCLUSTER_H
