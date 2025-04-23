#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <random>
#include "Header/combatSystem.h"
#include "Header/menuSystem.h"
#include "Items.h"

using namespace std;

// main combat loop
ocombat combat(GameState& game, int& enemyHealth, const std::string& enemyName)
{
    int defendCounter = 0;

    while (game.playerHealth > 0 && enemyHealth > 0)
    {
        cout << "=================================\n";
        cout << "           COMBAT MODE          \n";
        cout << "=================================\n\n";
        cout << "You are in a fight against: " << enemyName << "!\n";
        cout << game.playerName << " HP: " << game.playerHealth << "\n";
        cout << enemyName << " HP: " << enemyHealth << "\n\n";
        cout << "When ready, press ENTER";
        cin.ignore();
        clearScreen();

        vector<string> options = {"Attack", "Defend", "Retaliate", "Run"};
        int choice = displayMenu(options);

        switch (choice) {
        case 0: { // attack
                int damage = getRandomNumber(1, 35) + game.attack;
                enemyHealth -= damage;
                cout << "You attack for " << damage << " damage!\n";
                break;
        }
        case 1: { // defend
                cout << "You brace yourself!\n";
                defendCounter++;
                cout << "Charge stacks: " << defendCounter << "\n";
                break;
        }
        case 2: { // retaliate
                if (defendCounter > 0) {
                    int damage = static_cast<int>((defendCounter * 15) + 0.3 * game.attack + 0.5 * getRandomNumber(5, 20));
                    enemyHealth -= damage;
                    cout << "You retaliate, dealing " << damage << " damage!\n";
                    defendCounter = 0;
                } else {
                    cout << "You have no stored energy!\n";
                }
                break;
        }
        case 3: { // run
                cout << "You attempt to flee...\n";
                if (getRandomNumber(1, 100) >= 50) {
                    cout << "You escaped successfully!\n";
                    Sleep(1000);
                    return ocombat::Escaped;
                } else {
                    int damage = getRandomNumber(5, 20);
                    damage = max(0, damage - game.defense);
                    game.playerHealth -= damage;
                    cout << "You failed to escape! " << enemyName << " hits you for " << damage << "!\n";
                    continue;
                }
        }
        default:
            cout << "Invalid choice!\n";
            break;
        }

        // enemy turn
        if (enemyHealth > 0 && choice != 3) {
            int damage = getRandomNumber(5, 20);
            if (choice == 1) damage /= 2;
            damage = max(0, damage - game.defense);
            game.playerHealth -= damage;
            cout << enemyName << " hits you for " << damage << "!\n";
        }

        if (game.playerHealth <= 0) {
            cout << "You have been defeated!\n";
            return ocombat::Defeated;
        }
        if (enemyHealth <= 0) {
            cout << "You defeated " << enemyName << "!\n";
            // rewards
            int xpReward = getRandomNumber(20, 50);
            game.gainXP(xpReward); //add XP
            cout << "You gained " << xpReward << " XP!\n";
            game.levelUp(); //check and trigger level-up if ready

            int goldFound = getRandomNumber(10, 50);
            game.gold += goldFound;
            cout << "You obtained " << goldFound << " gold!\n";

            Item drop = generateRandomItem();
            game.inventory.push_back(drop);
            cout << "\nYou found: " << drop.name << " - " << drop.description << "\n";
            Sleep(1000);

            return ocombat::Victory;
        }
    }
        cout << "Combat Over!\nPress ENTER to continue...";
        cin.ignore();
        return ocombat::Victory;
    }

