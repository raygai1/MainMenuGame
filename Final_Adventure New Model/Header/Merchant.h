#ifndef MERCHANT_H
#define MERCHANT_H

#include "GameState.h"

class Merchant {
public:
    Merchant(); // Constructor

    // Display buff options as a menu, return chosen option
    int showBuffs() const;

    // Purchase the selected buff
    void purchaseBuff(GameState& game, int choice);

private:
    const int BUFF_COST = 100; // Each buff costs 100 gold
};

#endif // MERCHANT_H
