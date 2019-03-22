//
// Created by zhanghuimeng on 19-3-23.
//

#ifndef PLAYER_MOVE_H
#define PLAYER_MOVE_H

#include "GemCluster.h"
#include "Card.h"

class Move {
public:
    string player;
    enum Type {DIFF_COL, SAME_COL, RES1, RES2, BUY, BUY_RES} type;
    bool acqNobel;
    GemCluster acqGem;  // DIFF_COL & SAME_COL
    // TODO：log中出现res2情况怎么办？
    NormalCard card;  // RES1 & RES2

    Move(string _player, Type _type) {
        player = _player;
        type = _type;
    }
};


#endif //PLAYER_MOVE_H
