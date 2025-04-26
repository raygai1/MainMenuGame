#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <string>
#include "GameState.h"
#include "Environment.h" //for random number generation

class Enemy {
public:
    std::string name;
    int enemyHealth;
    int enemyAttack;
    int enemyAwareness;
    int xp;

    Enemy()
        : name("Unnamed"), enemyHealth(100), enemyAttack(10), enemyAwareness(50), xp(0) {} //base enemy constructor
    virtual ~Enemy() {} //virtual destructor for inheritance
};
//inheritance from Enemy to Boss
class Boss : public Enemy {
public:
    Boss() {
        name = "Ancient Beast"; //boss name
        enemyHealth = 300; //base boss health
        enemyAttack = 30;  //base boss attack
        enemyAwareness = 75; //boss higher awareness
        xp = 500; //high XP reward
    }
};

#endif
