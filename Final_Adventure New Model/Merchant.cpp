#include "Merchant.h"
#include "GameState.h"
#include "menuSystem.h"
#include <iostream>

const int BUFF_COST = 100;  //Cost for any merchant buff
Merchant::Merchant() {
    // Empty constructor for future use
}

int Merchant::showBuffs() const {
    std::vector<std::string> options = {
        "Health Buff (+10% max health)",
        "Attack Buff (+10% attack)",
        "Sneak Buff (+10% sneak skill)",
        "Flee Buff (+10% flee chance)",
        "Exit shop"
    };

    std::cout << "\n=== Merchant Buffs ===\n";
    std::cout << "Each buff costs 100 gold and increases the stat by 10%.\n";

    return displayMenu(options, "Choose a buff to purchase:");
}
void Merchant::purchaseBuff(GameState& game, int choice) {
    if (game.getGold() < BUFF_COST) {
        std::cout << "You don't have enough gold!\n";
        return;
    }
    switch (choice) {
        case 0: { //Health Buff
            int bonus = static_cast<int>(game.getMaxHealth() * 0.10);
            game.increaseMaxHealth(bonus);
            std::cout << "Your max HP increased by " << bonus << "!\n";
            break;
        }
        case 1: { //Attack Buff
            int bonus = static_cast<int>(game.getAttack() * 0.10);
            game.increaseAttack(bonus);
            std::cout << "Your attack increased by " << bonus << "!\n";
            break;
        }
        case 2: { //Sneak Buff
            int bonus = static_cast<int>(game.getSneak() * 0.10);
            game.increaseSneak(bonus);
            std::cout << "Your sneak increased by " << bonus << "!\n";
            break;
        }
    case 3: { //Run Buff
                game.increaseRunBonus(5); //+5 for better chance of running away
                std::cout << "Your escape ability increased!\n";
                break;
    }

        case 4: //Exit
            std::cout << "Thanks for stopping by!\n";
            return;
        default:
            std::cout << "Invalid choice.\n";
            return;
    }
    game.decreaseGold(BUFF_COST);
    std::cout << "Gold remaining: " << game.getGold() << "\n";
}
