#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "GameState.h"
extern int enemiesDefeated;
extern bool bossDefeated;
extern bool bossAvailable;

//outcome of combat
enum class ocombat {
    Victory,   //enemy defeated
    Escaped,   //player ran away
    Defeated,   //player lost
    BossTime    //Its boss time son
};
//main combat loop returns outcome
ocombat combat(GameState& game, int& enemyHealth, const std::string& enemyName);

#endif
