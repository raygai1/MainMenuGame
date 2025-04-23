#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "GameState.h"

// outcome of combat
enum class ocombat {
    Victory,   //enemy defeated
    Escaped,   //player ran away
    Defeated   //player lost
};

//main combat loop returns outcome
ocombat combat(GameState& game, int& enemyHealth, const std::string& enemyName);

#endif
