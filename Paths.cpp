#include "Header/Environment.h"
#include "Header/combatSystem.h"
#include "Header/menuSystem.h"
#include "GameState.h"
#include "Header/Enemy.h"
#include <iostream>
#include <vector>
#include <random>
#include <set>
using namespace std;

// forward declaration
void choosePathAndFight(GameState& game, int luck);

// sneak mechanic before combat
void sneak(GameState& game, int& awareness, const std::string& enemyName, bool& success) {
    int roll = getRandomNumber(1, 100);
    int score = game.playerSneak + roll;
    int threshold = awareness + 25;
    cout << "You attempt to sneak past " << enemyName << "...\n";
    cout << "Sneak Score: " << score << " vs Awareness: " << threshold << "\n";
    success = (score > threshold);
    cout << (success ? "You successfully sneak past!\n" : "You failed to sneak past!\n");
}

// pick a random environment
EnvironmentData generateEnvironment() {
    constexpr int total = sizeof(environments) / sizeof(environments[0]);
    return environments[getRandomNumber(0, total - 1)];
}

// pick a monster based on luck
string determineMonster(const EnvironmentData& env, int luck) {
    if (luck < 33) return env.monsters[0];
    if (luck < 66) return env.monsters[1];
    return env.monsters[2];
}

// main path chooser and combat trigger
void choosePathAndFight(GameState& game, int luck) {
    int pathCount = getRandomNumber(2, 5);
    vector<EnvironmentData> pathEnvs;
    vector<string> menuOptions;
    set<string> used;

    // build unique environments
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

    string enemyName = determineMonster(env, getRandomNumber(1, 100));
    clearScreen();
    cout << "A wild " << enemyName << " appears!\n";
    cout << "\nPress ENTER to prepare for action...";
    cin.ignore();
    clearScreen();

    //check stats
    clearScreen();
    GameState currentState;
    currentState = game; //sync current player object
    currentState.gold = game.gold; //sync gold
    currentState.inventory = game.inventory; //sync inventory if needed
    currentState.displayStats();
    std::cout << "\nPress ENTER to return...";
    std::cin.ignore();
    clearScreen();

    vector<string> preFightChoices = {"Sneak", "Swindle (Steal)", "Glory! (Fight)"};
    int choice = displayMenu(preFightChoices);

    int enemyHealth = 100 + static_cast<int>(game.level * 1.25);
    int enemyAwareness = 50;
    bool sneakSuccess = false;
    ocombat result = ocombat::Defeated;

    switch (choice) {
    case 0: { // Sneak
        sneak(game, enemyAwareness, enemyName, sneakSuccess);
        if (!sneakSuccess) result = combat(game, enemyHealth, enemyName);
        break;
    }
    case 1: { // Swindle
        int chance = getRandomNumber(1, 100);
        int difficulty = getRandomNumber(1, 100);
        if (chance >= difficulty) {
            int gold = getRandomNumber(10, 50);
            cout << "You successfully swindled " << gold << " gold from the " << enemyName << "!\n";
            game.gold += gold;
            game.saveToBinary("savegame_" + game.playerName + ".dat");
            cout << "Auto-saved game.\n";
            result = ocombat::Victory;
        } else {
            cout << "Swindle failed! The " << enemyName << " caught you!\n";
            result = combat(game, enemyHealth, enemyName);
        }
        cin.ignore();
        break;
    }
    case 2: { // Glory!
        cout << "You charge into battle with honor!\n";
        cin.ignore();
        result = combat(game, enemyHealth, enemyName);
        break;
    }
    }

    // post combat or successful sneak
    if (result == ocombat::Victory || sneakSuccess || result == ocombat::Escaped) {
        cout << "Victory! You move deeper into the world...\n";
        cin.ignore();

        // level up check
        int requiredXP = game.level * 10;
        if (game.xp >= requiredXP) game.levelUp();

        vector<string> postCombatOptions = { "Bonfire (Rest to heal your HP)", "Continue forward" };
        int restChoice = displayMenu(postCombatOptions);

        if (restChoice == 0) {
            int heal = 30 * game.level;
            game.healHP(heal);
            cout << "You rest and regain " << heal << " HP!\n";
        }

        cin.ignore();
        choosePathAndFight(game, luck);
    }
    else if (game.playerHealth <= 0) {
        cout << "You have fallen. Returning to menu...\n";
        cin.ignore();
    }
}
