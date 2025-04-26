#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>
#include "Environment.h"
#include "Items.h"

class GameState {
public:
    std::string playerName; //character name
    int level;              //current level
    int xp;                 //current XP
    int xpToNextLevel;      //XP needed to level up
    int playerHealth;       //current HP
    int attack;             //base attack stat
    int defense;            //reduces incoming damage
    int gold;               //currency
    int playerSneak;        //sneak chance
    int enemiesDefeated = 0; // counts regular enemy victories
    bool bossDefeated = false; // track if boss was already defeated
    //Merchant related
    int getGold() const;
    void decreaseGold(int amount);
    int getAttack() const;
    int getSneak() const;
    int getMaxHealth() const;
    int runBonus = 0; //integer added to Run roll
    int buffHealth = 0;
    int buffAttack = 0;
    int buffSneak = 0;
    int buffRun = 0;
    void increaseRunBonus(int amount); //adds to run chance
    void increaseAttack(int amount);
    void increaseSneak(int amount);
    void increaseMaxHealth(int amount);

    std::vector<Item> inventory; //held items

    GameState(); //sets default stats

    void saveToBinary(const std::string& filename);   //save to .dat
    bool loadFromBinary(const std::string& filename); //load .dat
    void levelUp(); //increase stats + auto-save
    void setPlayerName(); //input name
    void displayStats() const; //display all info
    void listSavedCharacters(); //show all .dat saves
    static std::vector<std::string> getSaveFilesInfo(); //get filenames, names & lvls

    int getHealth() const { return playerHealth; } //return HP
    int getAttack() { return getRandomNumber(1, 35) + attack; } //roll attack
    void healHP(int heal); //restore HP
    void takeDamage(int damage); //reduce HP from enemy
    void gainXP(int amount); //add XP
    void addGold(int amount); //earn gold
    void applyItem(const Item& item); //add item & apply stat effect
};
void displayStats(); //global display call
#endif
