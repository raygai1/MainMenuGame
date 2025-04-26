#include <iostream>    //standard input/output
#include <windows.h>   //windows Sleep function
#include <vector>      //vector container
#include <string>      //string objects
#include "GameState.h" //player stats & inventory
#include "Enemy.h"     //enemy & boss classes
#include "combatSystem.h" //combat function prototypes
#include "menuSystem.h"   //menu displays
#include "Items.h"        //item drops

using namespace std;

// Function declarations
void resetBuffs(GameState& game);               //reset temporary buffs
void playerAttack(GameState& game, int& enemyHealth); //normal attack
void playerDefend(int& defendCounter);           //defend and charge retaliate
void playerRetaliate(GameState& game, int& enemyHealth, int& defendCounter); //special retaliation
bool attemptRun(GameState& game, const std::string& enemyName); //try to flee
bool checkBossSpawn(GameState& game);            //checks if boss fight should start

// Main combat loop
ocombat combat(GameState& game, int& enemyHealth, const std::string& enemyName)
{
    int defendCounter = 0;//counter for retaliation damage
    int baseAttack = game.attack + game.buffAttack; //total attack power with buffs

    while (game.playerHealth > 0 && enemyHealth > 0) //combat loop while alive
    {
        //combat screen
        cout << "=================================\n";
        cout << "           COMBAT MODE          \n";
        cout << "=================================\n\n";
        cout << "You are fighting: " << enemyName << "\n";
        cout << game.playerName << " HP: " << game.playerHealth << "\n";
        cout << enemyName << " HP: " << enemyHealth << "\n\n";
        cout << "Press ENTER to continue...\n";
        cin.ignore();
        clearScreen();

        //menu of combat choices
        vector<string> options = {"Attack", "Defend", "Retaliate", "Run"};
        int choice = displayMenu(options);

        switch (choice) {
        case 0: playerAttack(game, enemyHealth); break; //normal attack
        case 1: playerDefend(defendCounter); break;     //defend
        case 2: playerRetaliate(game, enemyHealth, defendCounter); break; //retaliate
        case 3: //run
            if (attemptRun(game, enemyName)) {
                resetBuffs(game);
                return ocombat::Escaped;
            }
            continue;
        default:
            cout << "Invalid choice!\n";
            break;
        }

        //enemy counterattack
        if (enemyHealth > 0 && choice != 3) {
            int damage = getRandomNumber(5, 20) + (1.5 * game.level);
            if (choice == 1) damage /= 3;              //defense halves enemy damage
            damage = max(0, damage - game.defense);    //apply player defense
            game.playerHealth -= damage;
            cout << enemyName << " hits you for " << damage << "!\n";
        }

        //player defeated
        if (game.playerHealth <= 0) {
            cout << "You have been defeated!\n";
            resetBuffs(game);
            return ocombat::Defeated;
        }

        //enemy defeated
        if (enemyHealth <= 0) {
            cout << "You defeated " << enemyName << "!\n";

            //rewards
            int xpReward = getRandomNumber(20, 50);
            game.gainXP(xpReward);
            int goldFound = getRandomNumber(10, 50);
            game.addGold(goldFound);

            //drop item
            Item drop = generateRandomItem();
            game.inventory.push_back(drop);
            cout << "You found: " << drop.name << " - " << drop.description << "\n";
            cin.ignore();

            game.enemiesDefeated++; //count victories

            //⬇ Check boss spawn
            if (checkBossSpawn(game)) {
                resetBuffs(game);
                return ocombat::BossTime;
            }

            resetBuffs(game); //clear buffs after normal combat
            return ocombat::Victory;
        }
    }
    return ocombat::Victory; //failsafe return
}

// Reset all temporary buffs
void resetBuffs(GameState& game) {
    game.buffHealth = 0;
    game.buffAttack = 0;
    game.buffSneak = 0;
    game.buffRun = 0;
}

// Normal attack
void playerAttack(GameState& game, int& enemyHealth) {
    int baseAttack = game.attack + game.buffAttack;
    int damage = getRandomNumber(1, 35) + baseAttack;
    enemyHealth -= damage;
    cout << "You attack for " << damage << " damage!\n";
}

// Defensive stance to charge retaliate
void playerDefend(int& defendCounter) {
    cout << "You brace yourself!\n";
    defendCounter++;
    cout << "Charge stacks: " << defendCounter << "\n";
}

// Retaliate attack using stored charge
void playerRetaliate(GameState& game, int& enemyHealth, int& defendCounter) {
    int baseAttack = game.attack + game.buffAttack;
    if (defendCounter > 0) {
        int damage = static_cast<int>((defendCounter * 15) + (0.5 * baseAttack) + (0.5 * getRandomNumber(5, 20)));
        enemyHealth -= damage;
        cout << "You retaliate, dealing " << damage << " damage!\n";
        defendCounter = 0; //reset after retaliation
    } else {
        cout << "You have no stored energy!\n";
    }
}

// Attempt to flee from battle
bool attemptRun(GameState& game, const std::string& enemyName) {
    cout << "You attempt to flee...\n";
    int roll = getRandomNumber(1, 100) + game.runBonus; //buffed escape roll
    if (roll >= 50) {
        cout << "You escaped successfully!\n";
        Sleep(1000);
        return true;
    } else {
        int damage = getRandomNumber(5, 20);
        damage = max(0, damage - game.defense);
        game.playerHealth -= damage;
        cout << "Failed to flee! " << enemyName << " hits you for " << damage << "!\n";
        return false;
    }
}

// Check if boss should spawn after 10 wins
bool checkBossSpawn(GameState& game) {
    if (game.enemiesDefeated > 0 && game.enemiesDefeated % 10 == 0 && !game.bossDefeated) {
        cout << "\nA monstrous roar shakes the world...\n";

        Boss finalBoss; //create boss
        finalBoss.name = "Ancient Beast";
        finalBoss.enemyHealth = 300;
        finalBoss.enemyAttack = 30;
        finalBoss.enemyAwareness = 75;

        int bossHealth = finalBoss.enemyHealth;
        ocombat bossOutcome = combat(game, bossHealth, finalBoss.name);

        if (bossOutcome == ocombat::Victory) {
            game.bossDefeated = true;
            cout << "You've conquered the Ancient Beast!\n";
            game.addGold(500);
            return true;
        } else {
            cout << "You were slain by the boss...\n";
            return true;
        }
    }
    return false;
}
