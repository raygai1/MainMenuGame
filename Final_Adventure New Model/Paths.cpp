#include "Environment.h" //random environments
#include "combatSystem.h" //combat loop
#include "menuSystem.h"   //menu display
#include "GameState.h"    //player state
#include "Enemy.h"        //enemy & boss classes
#include "Merchant.h"     //merchant system
#include <iostream>       //input/output
#include <vector>         //dynamic arrays
#include <set>            //unique environment choices
using namespace std;

// Forward declaration
void choosePathAndFight(GameState& game, int luck);

// Sneak mechanic before combat
void sneak(GameState& game, int& awareness, const std::string& enemyName, bool& success) {
    int roll = getRandomNumber(1, 100);
    int score = game.playerSneak + roll;
    int threshold = awareness + 25;
    cout << "You attempt to sneak past " << enemyName << "...\n";
    cout << "Sneak Score: " << score << " vs Awareness: " << threshold << "\n";
    success = (score > threshold);
    cout << (success ? "You successfully sneak past!\n" : "You failed to sneak past!\n");
}

// Pick random environment
EnvironmentData generateEnvironment() {
    constexpr int total = sizeof(environments) / sizeof(environments[0]);
    return environments[getRandomNumber(0, total - 1)];
}

// Pick random monster based on environment and luck
string determineMonster(const EnvironmentData& env, int luck) {
    if (luck < 33) return env.monsters[0];
    if (luck < 66) return env.monsters[1];
    return env.monsters[2];
}
string determineBoss(const EnvironmentData& env, int luck) {
    if (luck < 33) return env.monsters[3];
    if (luck < 66) return env.monsters[4];
    return env.monsters[5];
}

// Core path choosing + pre-fight options + combat resolution
void choosePathAndFight(GameState& game, int luck) {
    int pathCount = getRandomNumber(2, 5);
    vector<EnvironmentData> pathEnvs;
    vector<string> menuOptions;
    set<string> used;

    // Build unique environments
    while (pathEnvs.size() < pathCount) {
        auto env = generateEnvironment();
        if (used.insert(env.description).second) {
            pathEnvs.push_back(env);
            menuOptions.push_back(env.description);
        }
    }

    int selectedPath = displayMenu(menuOptions, "\nChoose a path:");
    EnvironmentData env = pathEnvs[selectedPath];

    clearScreen();
    cout << "You walk down the path...\n";
    cout << "Environment: " << env.description << "\n";
    cin.ignore();

    // Generate enemy encounter
    string enemyName = determineMonster(env, getRandomNumber(1, 100));
    clearScreen();
    cout << "A wild " << enemyName << " appears!\n";
    cout << "\nPress ENTER to prepare for action...";
    cin.ignore();
    clearScreen();

    // Quick stat check screen
    clearScreen();
    game.displayStats();
    cout << "\nPress ENTER to return...";
    cin.ignore();
    clearScreen();

    int enemyHealth = 100 + static_cast<int>(game.level * 1.25);
    int enemyAwareness = 50;
    bool sneakSuccess = false;
    ocombat result = ocombat::Defeated;

    // Pre-fight action menu
    bool actionChosen = false;
    while (!actionChosen) {
        vector<string> preFightChoices = {
            "Sneak",
            "Swindle (Steal)",
            "Glory! (Fight)",
            "Visit Merchant (Buy Buffs)"
        };
        int choice = displayMenu(preFightChoices);

        switch (choice) {
        case 0: { // Sneak attempt
            sneak(game, enemyAwareness, enemyName, sneakSuccess);
            if (!sneakSuccess) result = combat(game, enemyHealth, enemyName);
            actionChosen = true;
            break;
        }
        case 1: { // Swindle attempt
            int chance = getRandomNumber(1, 100);
            int difficulty = getRandomNumber(1, 100);
            if (chance >= difficulty) {
                int gold = getRandomNumber(10, 50);
                cout << "You swindled " << gold << " gold from the " << enemyName << "!\n";
                game.addGold(gold);
                result = ocombat::Victory;
            } else {
                cout << "Swindle failed! The " << enemyName << " caught you!\n";
                result = combat(game, enemyHealth, enemyName);
            }
            cin.ignore();
            actionChosen = true;
            break;
        }
        case 2: { // Glory fight
            cout << "You charge into battle!\n";
            cin.ignore();
            result = combat(game, enemyHealth, enemyName);
            actionChosen = true;
            break;
        }
        case 3: { // Visit merchant
            Merchant merchant;
            int buffChoice = merchant.showBuffs();
            if (buffChoice != 5) {
                merchant.purchaseBuff(game, buffChoice);
                cout << "\nPress ENTER to continue...";
                cin.ignore();
                cin.get();
            }
            break; //stay in loop until action chosen
        }
        }
    }

    // Post-combat resolution
    if (result == ocombat::Victory || sneakSuccess || result == ocombat::Escaped) {
        cout << "Victory! You move deeper...\n";
        cin.ignore();

        if (game.xp >= game.xpToNextLevel)
            game.levelUp();

        // Post-fight menu (after normal enemy)
        vector<string> postFightOptions = {
            "Bonfire (Heal)",
            "Visit Merchant",
            "Continue exploring"
        };
        int postChoice = displayMenu(postFightOptions);

        switch (postChoice) {
        case 0: { // Bonfire
            int heal = 40 * game.level;
            game.healHP(heal);
            cout << "You rest and heal " << heal << " HP.\n";
            cin.ignore();
            break;
        }
        case 1: { // Merchant
            Merchant merchant;
            int buffChoice = merchant.showBuffs();
            if (buffChoice != 5) {
                merchant.purchaseBuff(game, buffChoice);
                cout << "\nPress ENTER to continue...";
                cin.ignore();
                cin.get();
            }
            break;
        }
        case 2: // Continue exploring
            break;
        }

        // Reset buffs after combat
        game.buffHealth = 0;
        game.buffAttack = 0;
        game.buffSneak = 0;
        game.buffRun = 0;

        // Boss spawn check
        if (game.enemiesDefeated > 0 && game.enemiesDefeated % 2 == 0 && !game.bossDefeated) {
            cout << "\nYou hear a powerful roar...\n";
            Boss finalBoss;
            finalBoss.name = determineBoss(env,getRandomNumber(1,100));
            finalBoss.enemyHealth = 200 + (0.75 * game.level);
            finalBoss.enemyAttack = getRandomNumber(0,80) + (0.5 * game.level);
            finalBoss.enemyAwareness = getRandomNumber(0,100);

            int bossHealth = finalBoss.enemyHealth;
            ocombat bossResult = combat(game, bossHealth, finalBoss.name);

            if (bossResult == ocombat::Victory || bossResult == ocombat::BossTime) {
                cout << "You have slain the Ancient Beast!\n";
                game.addGold(500);
                game.bossDefeated = true;
                game.enemiesDefeated = 0;
                game.saveToBinary("savegame_" + game.playerName + ".dat");
                choosePathAndFight(game, game.level);
                return;
            } else {
                cout << "You were slain by the boss...\n";
                game.enemiesDefeated = 0;
                return;
            }
        }

        // Continue exploration
        choosePathAndFight(game, luck);
        return;
    }
    if (result == ocombat::BossTime) {
        cout << "Boss battle finished. Returning to adventure...\n";
        choosePathAndFight(game, luck);
        return;
    }
    if (game.playerHealth <= 0) {
        cout << "You have fallen. Returning to menu...\n";
        cin.ignore();
    }
}
