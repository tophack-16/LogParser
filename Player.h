//
// Created by zhanghuimeng on 19-3-22.
//

#ifndef PLAYER_USER_H
#define PLAYER_USER_H

#include "GemCluster.h"
#include "Card.h"

using namespace std;

class Player {
    string name;
    int score;
    GemCluster gems;
    vector<NormalCard> purchased_cards;
    vector<NormalCard> reserved_cards;
    vector<NobelCard> nobles;
};


#endif //PLAYER_USER_H
