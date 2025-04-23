#ifndef GAME_HEADER_H
#define GAME_HEADER_H

#include "Header/Environment.h"
#include "Header/Player.h"
#include "Header/combatSystem.h"
#include "Header/menuSystem.h"
#include "Header/Enemy.h"
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// Forward declaration of Boss class (assuming it's defined elsewhere or will be)
class boss : public Enemy {
public:
    boss(); // Constructor
    // Add any unique boss properties or methods here
};

// Generate a random number within a given range
int getRandomNumber(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

// Generate a random environment from the predefined list
EnvironmentData generateEnvironment() {
    constexpr int numEnvironments = sizeof(environments) / sizeof(environments[0]);
    int index = getRandomNumber(0, numEnvironments - 1);
    return environments[index];
}

// Select a monster based on environment and luck
string determineMonster(const EnvironmentData& env, int luck) {
    if (luck < 33) return env.monsters[0];
    if (luck < 66) return env.monsters[1];
    return env.monsters[2];
}

// Global counter for defeated enemies
int defeatedEnemyCounter = 0;
bool bossSpawned = false;

// Function to handle boss spawning
Enemy* maybeSpawnBoss() {
    if (defeatedEnemyCounter >= 10 && !bossSpawned) {
        bossSpawned = true;
        cout << "\n\n************************************\n";
        cout << "A powerful foe emerges! The BOSS has appeared!\n";
        cout << "************************************\n";
        cin.ignore();
        Boss* boss = new Boss(); // Create a new Boss object
        boss->name = "Grand Overlord"; // Example boss name
        boss->level = 5 + (rand() % 3); // Example boss level scaling
        boss->enemyHealth = 300 + (boss->level * 20); // Example boss health scaling
        boss->enemyAttack = 20 + (boss->level * 3);   // Example boss attack scaling
        boss->enemyAwareness = 70;                  // Example boss awareness
        return boss;
    }
    return nullptr;
}

// Path selection and combat
void choosePathAndFight(Player& player, int luck) {
    Enemy* currentEnemy = nullptr; // Pointer to the current enemy (regular or boss)

    // Check if the boss should spawn
    Enemy* potentialBoss = maybeSpawnBoss();
    if (potentialBoss != nullptr) {
        currentEnemy = potentialBoss;
    } else {
        int pathCount = getRandomNumber(2, 5);
        vector<EnvironmentData> pathEnvironments;
        vector<string> menuOptions;

        // Generate random paths
        for (int i = 0; i < pathCount; i++) {
            EnvironmentData env = generateEnvironment();
            pathEnvironments.push_back(env);
            menuOptions.push_back(env.description);
        }

        // **Display menu with "Choose a path:" properly positioned**
        int selectedPath = displayMenu(menuOptions, "\nChoose a path:");

        // Get selected environment
        EnvironmentData chosenEnv = pathEnvironments[selectedPath];

        clearScreen();
        cout << "You walk down the path...\n";
        cout << "Environment: " << chosenEnv.description << "\n";
        cin.ignore();

        // Generate regular enemy based on the chosen environment
        string enemyName = determineMonster(chosenEnv, getRandomNumber(1, 100));
        clearScreen();
        cout << "A wild " << enemyName << " appears!\n";
        cout << "\nPress ENTER to prepare for action...";
        std::cin.ignore(); // Waits for Enter key
        clearScreen(); // Then clears

        currentEnemy = new Enemy(); // Create a new regular Enemy object
        currentEnemy->name = enemyName;
        currentEnemy->level = player.level + (rand() % 2) - 1; // Scale enemy level
        if (currentEnemy->level < 1) currentEnemy->level = 1;
        currentEnemy->enemyHealth = 100 + (currentEnemy->level * 15);
        currentEnemy->enemyAttack = 10 + (currentEnemy->level * 2);
        currentEnemy->enemyAwareness = 50 + (currentEnemy->level * 3);
    }

    if (currentEnemy != nullptr) {
        std::vector<std::string> preFightChoices;
        bool canSneak = true; // Add logic to determine if sneaking is possible against this enemy

        if (canSneak) {
            preFightChoices.push_back("Sneak");
        }
        preFightChoices.push_back("Swindle");
        preFightChoices.push_back("Glory");

        displayMenu(preFightChoices);


        combat(player, currentEnemy->enemyHealth, currentEnemy->name);

        // post combat actions
        if (currentEnemy->enemyHealth <= 0) {
            cout << "Victory! You move deeper into the world...\n";
            cin.ignore();

            if (!bossSpawned) {
                defeatedEnemyCounter++; // Increment the counter for defeated enemies
            }

            // bonfire option
            vector<string> postCombatOptions = { "Bonfire (Rest to heal your HP)", "Continue forward" };
            int choice2 = displayMenu(postCombatOptions);

            switch (choice2) {
                case 0: { // rest option
                    int healAmount = 30 * (player.level * .5);
                    player.healHP(healAmount);
                    cout << "You rest and regain " << healAmount << " HP!\n";
                    break;
                }
                case 1: {
                    // continue
                    choosePathAndFight(player, luck);
                    break;
                }
            }
            cin.ignore();
            if (!bossSpawned) {
                choosePathAndFight(player, luck); // Continue to the next encounter if not a boss
            }

        } else {
            cout << "You have fallen. Returning to menu...\n";
            cin.ignore();
        }
        delete currentEnemy; // Clean up the dynamically allocated enemy
    }
}

#endif // GAME_HEADER_H