#include "Merchant.h"
#include "GameState.h"
#include "menuSystem.h"
#include <iostream>
#include "Items.h"

using namespace std;

Merchant::Merchant() {
    // Empty constructor for future use
}
// Cost scales with player level
int Merchant::calculateBuffCost(GameState& game) const {
    int baseCost = 100;
    int levelMultiplier = 15; // Costs go up 15 gold per level
    return baseCost + (game.level * levelMultiplier);
}

int Merchant::showBuffs(GameState& game) const {
    vector<string> options = {
        "Health Buff (+10% max health)",
        "Attack Buff (+10% attack)",
        "Sneak Buff (+10% sneak skill)",
        "Flee Buff (+10% flee chance)",
        "Random Weapon",
        "Exit shop"
    };
    displayMenu(options, "    Merchant Buffs\n    Each buff costs " + to_string(calculateBuffCost(game)) + " gold.");
    return 0;
}

void Merchant::purchaseBuff(GameState& game, int choice) {
    int buffCost = calculateBuffCost(game);

    if (game.getGold() < buffCost) {
        cout << "You don't have enough gold!\n";
        return;
    }
    switch (choice) {
        case 0: { // Health Buff
            int bonus = static_cast<int>(game.getMaxHealth() * 0.10);
            game.increaseMaxHealth(bonus);
            cout << "Your max HP increased by " << bonus << "!\n";
            break;
        }
        case 1: { // Attack Buff
            int bonus = static_cast<int>(game.getAttack() * 0.10);
            game.increaseAttack(bonus);
            cout << "Your attack increased by " << bonus << "!\n";
            break;
        }
        case 2: { // Sneak Buff
            int bonus = static_cast<int>(game.getSneak() * 0.10);
            game.increaseSneak(bonus);
            cout << "Your sneak increased by " << bonus << "!\n";
            break;
        }
        case 3: { // Flee Buff
            game.increaseRunBonus(5); // +5 flee chance
            cout << "Your escape ability increased!\n";
            break;
        }
        case 4: { // Random Weapon
            Item drop = generateRandomItem();
            game.inventory.push_back(drop);
            cout << "You bought: " << drop.name << " - " << drop.description << "\n";
            break;
        }
        case 5: // Exit
            cout << "Thanks for stopping by!\n";
            return;
        default:
            cout << "Invalid choice.\n";
            return;
    }
    game.decreaseGold(buffCost);
    cout << "Gold remaining: " << game.getGold() << "\n";
}
