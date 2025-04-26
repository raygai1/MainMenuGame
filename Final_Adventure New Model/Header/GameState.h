#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>   //vector container
#include <string>   //string objects
#include "Environment.h" //for random number and environments
#include "Items.h"       //item types and inventory

// Stores player stats, inventory, buffs
class GameState {
public:
    std::string playerName;   //character name
    int level;                //current level
    int xp;                   //current XP
    int xpToNextLevel;        //XP needed to level up
    int playerHealth;         //current HP
    int maxHealth;            //maximum HP (for healing)
    int attack;               //base attack power
    int defense;              //defense stat (reduces damage)
    int gold;                 //currency
    int playerSneak;          //sneak skill
    int enemiesDefeated = 0;  //number of normal enemies defeated
    bool bossDefeated = false; //true if boss was beaten

    // Merchant temporary buffs
    int runBonus = 0;         //permanent flee bonus
    int buffHealth = 0;       //temporary HP buff
    int buffAttack = 0;       //temporary attack buff
    int buffSneak = 0;        //temporary sneak buff
    int buffRun = 0;          //temporary flee buff

    std::vector<Item> inventory; //player's items

    // Constructor - sets default starting stats
    GameState();

    // Save and load
    void saveToBinary(const std::string& filename);   //save player data
    bool loadFromBinary(const std::string& filename); //load player data

    // Core functions
    void levelUp();               //level up if enough XP
    void setPlayerName();          //name input
    void displayStats() const;     //show player stats
    void listSavedCharacters();    //list saved characters
    static std::vector<std::string> getSaveFilesInfo(); //list save files info

    // Combat accessors
    int getHealth() const { return playerHealth; } //return current HP
    int getAttack() { return getRandomNumber(1, 35) + attack; } //return random attack roll
    int getMaxHealth() const { return maxHealth; } //return max HP
    int getGold() const; //return gold
    int getSneak() const; //return sneak value

    // Combat modifiers
    void healHP(int heal);      //heal HP
    void takeDamage(int damage); //lose HP from attack
    void gainXP(int amount);    //gain XP
    void addGold(int amount);   //gain gold
    void applyItem(const Item& item); //apply item effects

    // Buff-related changes
    void increaseAttack(int amount);      //buff attack
    void increaseSneak(int amount);        //buff sneak
    void increaseMaxHealth(int amount);    //buff max HP
    void increaseRunBonus(int amount);     //buff run chance
    void decreaseGold(int amount); //decrease player's gold
};

//external global function
void displayStats(); //global stat display

#endif
