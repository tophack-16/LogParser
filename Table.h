//
// Created by zhanghuimeng on 19-3-23.
//

#ifndef PLAYER_TABLE_H
#define PLAYER_TABLE_H

#include "GemCluster.h"
#include "Card.h"

class Table {
    GemCluster gems;
    vector<NormalCard> cards;
    vector<NobelCard> nobles;
};


#endif //PLAYER_TABLE_H
