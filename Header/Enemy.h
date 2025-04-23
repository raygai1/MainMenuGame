#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <string>

class Enemy {
public:
    std::string name;
    int level;
    int enemyHealth;
    int xp;
    int xpToNextLevel;
    int enemyAttack;
    int enemyAwareness;

    //constructor
    Enemy() : name("Unnamed Enemy"), level(1), enemyHealth(100), xp(0), xpToNextLevel(45), enemyAttack(10), enemyAwareness(50) {}
    virtual ~Enemy() {} //virtual destructor for proper inheritance cleanup
};

class Boss : public Enemy { //inheritance from Enemy data
public:
    Boss() {
        name = "Unnamed Boss";
        level = 5; // Example base boss level
        enemyHealth = 300; // Example base boss health
        xp = 100;    // Example boss XP reward
        xpToNextLevel = 0; // Bosses don't typically level up
        enemyAttack = 25;  // Example base boss attack
        enemyAwareness = 70; // Example base boss awareness
    }
};

#endif // ENEMY_H