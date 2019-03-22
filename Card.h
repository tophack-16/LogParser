//
// Created by zhanghuimeng on 19-3-22.
//

#ifndef PLAYER_CARD_H
#define PLAYER_CARD_H

#include <string>
#include "GemCluster.h"

using namespace std;

class Card {
private:
    int score;

    void gemsParser() {

    }

public:
    Card(int _score = 0) {
        score = _score;
    }
};

class NormalCard: Card {
    int level;
    GemCluster color;
    GemCluster costs;
};

class NobelCard: Card {
    GemCluster requirements;
};


#endif //PLAYER_CARD_H
