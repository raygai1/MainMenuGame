#ifndef MERCHANT_H
#define MERCHANT_H

#include "GameState.h"

class Merchant {
public:
    Merchant(); // Constructor

    // Display buff options as a menu, return chosen option
    int showBuffs(GameState& game) const;

    // Purchase the selected buff
    void purchaseBuff(GameState& game, int choice);

private:
    int calculateBuffCost(GameState& game) const;

};

#endif // MERCHANT_H
