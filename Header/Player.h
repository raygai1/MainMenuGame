#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>       //player name & strings
#include <vector>       //player inventory
#include "Environment.h" //for getRandomNumber()
#include "Items.h"       //item drops and effects
#include "GameState.h"

//stores player stats, leveling, combat, and inventory
class Player
{
public:
    std::string name;    //player name

    int level;           //current player level
    int playerHealth;    //current HP
    int xp;              //current XP
    int xpToNextLevel;   //XP needed to level up
    int attack;          //attack power
    int playerSneak;     //sneak chance
    int defense;         //reduces incoming damage
    int gold;            //currency

    std::vector<Item> inventory; //player's items

    Player() {     //constructor for base stats
        level = 1;
        playerHealth = 100;
        xp = 0;
        xpToNextLevel = requiredXP;
        attack = 10;
        defense = 0;
        playerSneak = game.level * 3;
        gold = 0;
    }
    int getHealth() const {     //get current HP
        return Health;
    }
    int getAttack() { //randomize player attack + base attack
        return getRandomNumber(1, 35) + attack;
    }
    void healHP(int heal) { //increase HP by amount (max 100)
        Health += heal;
        if (Health > 100) Health = 100;
        std::cout << "You healed for " << heal << " HP! Current HP: " << Health << std::endl;
    }
    void takeDamage(int damage) { //take damage with defense reduction
        int reduced = damage - defense;
        if (reduced < 0) reduced = 0;
        Health -= reduced;
        if (Health < 0) Health = 0;
        std::cout << "You took " << reduced << " damage! Current HP: " << Health << std::endl;
    }
    void gainXP(int amount) { //add XP and check for level up
        xp += amount;
        std::cout << "You gained " << amount << " XP! Total XP: " << xp << std::endl;
        while (xp >= requiredXP) {
            levelUp();
        }
    }
    void levelUp() { //increase level, HP, attack, XP scale
        xp -= requiredXP;
        level++;
        requiredXP += level * 30;
        Health += level * 20;
        attack += level * 10;

        std::cout << "Congratulations! You leveled up!" << std::endl;
        std::cout << "Level: " << level << " | HP: " << playerHealth << " | Atk: " << attack << std::endl;
    }*/
    void addGold(int amount) { //add gold from swindle/combat
        gold += amount;
        std::cout << "You gained " << amount << " gold!\n";
    }
    void applyItem(const Item& item) { //applies item effects and adds to inventory
        inventory.push_back(item);
        switch (item.type) {
        case ItemType::Weapon:
            attack += item.statEffect; //+atk
            break;
        case ItemType::Armor:
            defense += item.statEffect; //+def
            break;
        case ItemType::CursedWeapon:
            attack += item.statEffect; //-def
            break;
        case ItemType::CursedArmor:
            defense += item.statEffect; //-def
            break;
        default:
            break; //funny item=0
        }
    }
};
//sneak function prototype
void sneak(Player& player, int& enemyAwareness, const std::string& enemyName, bool& sneakSuccess);

#endif
