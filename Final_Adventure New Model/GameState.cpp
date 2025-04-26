#include "GameState.h"
#include <iostream> //standard output
#include "menuSystem.h" //menus
#include <fstream> //file saving
#include <filesystem> //for listing save files
namespace fs = std::filesystem;

extern GameState currentGame; //global current player info

//Constructor - sets default starting values
GameState::GameState()
    : playerName("Unknown"), level(1), xp(0), xpToNextLevel(45),
      playerHealth(100), maxHealth(100),
      attack(10), defense(0), gold(0), playerSneak(3),
      buffHealth(0), buffAttack(0), buffSneak(0), buffRun(0),
      enemiesDefeated(0), bossDefeated(false)
{}

//Save all important player info to a .dat file
void GameState::saveToBinary(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open file for saving.\n";
        return;
    }
    //save playerName
    size_t nameLength = playerName.size();
    outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    outFile.write(playerName.c_str(), nameLength);
    //save base stats
    outFile.write(reinterpret_cast<const char*>(&level), sizeof(level));
    outFile.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    outFile.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
    outFile.write(reinterpret_cast<const char*>(&attack), sizeof(attack));
    outFile.write(reinterpret_cast<const char*>(&defense), sizeof(defense));
    outFile.write(reinterpret_cast<const char*>(&gold), sizeof(gold));
    outFile.write(reinterpret_cast<const char*>(&xp), sizeof(xp));
    outFile.write(reinterpret_cast<const char*>(&xpToNextLevel), sizeof(xpToNextLevel));

    //save inventory
    size_t inventorySize = inventory.size();
    outFile.write(reinterpret_cast<const char*>(&inventorySize), sizeof(inventorySize));
    for (const auto& item : inventory) {
        size_t nameLen = item.name.size();
        size_t descLen = item.description.size();
        int typeInt = static_cast<int>(item.type);

        outFile.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        outFile.write(item.name.c_str(), nameLen);

        outFile.write(reinterpret_cast<const char*>(&descLen), sizeof(descLen));
        outFile.write(item.description.c_str(), descLen);

        outFile.write(reinterpret_cast<const char*>(&typeInt), sizeof(typeInt));
        outFile.write(reinterpret_cast<const char*>(&item.statEffect), sizeof(item.statEffect));
    }

    std::cout << "Game saved successfully!\n";
}

//Load player info from a .dat file
bool GameState::loadFromBinary(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open file for loading.\n";
        return false;
    }

    //load playerName
    size_t nameLength;
    inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    playerName.resize(nameLength);
    inFile.read(&playerName[0], nameLength);

    //load base stats
    inFile.read(reinterpret_cast<char*>(&level), sizeof(level));
    inFile.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    inFile.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
    inFile.read(reinterpret_cast<char*>(&attack), sizeof(attack));
    inFile.read(reinterpret_cast<char*>(&defense), sizeof(defense));
    inFile.read(reinterpret_cast<char*>(&gold), sizeof(gold));
    inFile.read(reinterpret_cast<char*>(&xp), sizeof(xp));
    inFile.read(reinterpret_cast<char*>(&xpToNextLevel), sizeof(xpToNextLevel));

    //load inventory
    size_t inventorySize;
    inFile.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));
    inventory.clear();
    for (size_t i = 0; i < inventorySize; ++i) {
        size_t nameLen, descLen;
        inFile.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        std::string name(nameLen, '\0');
        inFile.read(&name[0], nameLen);

        inFile.read(reinterpret_cast<char*>(&descLen), sizeof(descLen));
        std::string desc(descLen, '\0');
        inFile.read(&desc[0], descLen);

        int typeInt;
        inFile.read(reinterpret_cast<char*>(&typeInt), sizeof(typeInt));
        int effect;
        inFile.read(reinterpret_cast<char*>(&effect), sizeof(effect));

        inventory.emplace_back(name, desc, static_cast<ItemType>(typeInt), effect);
    }

    std::cout << "Game loaded successfully!\n";
    return true;
}
// Get current gold
int GameState::getGold() const {
    return gold;
}

// Increase player's maximum health (adds to playerHealth)
void GameState::increaseMaxHealth(int amount) {
    playerHealth += amount;
}

// Increase player's base attack
void GameState::increaseAttack(int amount) {
    attack += amount;
}

// Get current sneak value
int GameState::getSneak() const {
    return playerSneak;
}

// Increase sneak stat
void GameState::increaseSneak(int amount) {
    playerSneak += amount;
}

// Increase run bonus for better escape chances
void GameState::increaseRunBonus(int amount) {
    runBonus += amount;
}
//add XP and print total
void GameState::gainXP(int amount) {
    xp += amount;
    std::cout << "You gained " << amount << " XP! Total XP: " << xp << "\n";
}

//Handle Leveling Up if enough XP
void GameState::levelUp() {
    while (xp >= xpToNextLevel) {
        xp -= xpToNextLevel;
        level++;
        xpToNextLevel += level * 30;

        //increase stats
        maxHealth += level * 20;
        playerHealth = maxHealth; //heal to full
        attack += level * 10;
        defense += level * 5;

        std::cout << "Level Up! Now Level " << level
                  << " | HP: " << playerHealth << "/" << maxHealth
                  << " | Attack: " << attack
                  << " | Defense: " << defense << "\n";

        saveToBinary("savegame_" + playerName + ".dat"); //auto-save after leveling
    }
}

//heal player HP (max to maxHealth)
void GameState::healHP(int amount) {
    playerHealth += amount;
    if (playerHealth > maxHealth) playerHealth = maxHealth;
    std::cout << "You healed for " << amount << " HP! Current HP: " << playerHealth << "/" << maxHealth << "\n";
}

//Prompt for player name input
void GameState::setPlayerName() {
    std::cout << "Enter your character's name (letters/spaces only): ";
    std::getline(std::cin, playerName);

    for (char& c : playerName) {
        if (!isalpha(c) && !isspace(c)) {
            std::cout << "Invalid name. Using default: Unknown\n";
            playerName = "Unknown";
            return;
        }
    }
    if (playerName.empty()) playerName = "Unknown";
}

//Display all player stats cleanly
void GameState::displayStats() const {
    std::cout << "=== Character Stats ===\n";
    std::cout << "Name: " << playerName << "\n";
    std::cout << "Level: " << level
              << " | Health: " << playerHealth << "/" << maxHealth
              << " | Gold: " << gold << "\n";
    std::cout << "XP: " << xp << " / " << xpToNextLevel << "\n";
    std::cout << "Attack: " << attack << " | Defense: " << defense << "\n";
    std::cout << "Inventory:\n";
    for (const auto& item : inventory)
        std::cout << " - " << item.name << " (" << item.description << ")\n";
}

//add gold from fights, events, or loot
void GameState::addGold(int amount) {
    gold += amount;
    std::cout << "You gained " << amount << " gold!\n";
}

//get .dat save files and show their info
std::vector<std::string> GameState::getSaveFilesInfo() {
    std::vector<std::string> saveInfo;
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".dat") {
            std::ifstream inFile(entry.path(), std::ios::binary);
            if (inFile) {
                size_t nameLength;
                inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                std::string name(nameLength, '\0');
                inFile.read(&name[0], nameLength);

                int lvl;
                inFile.read(reinterpret_cast<char*>(&lvl), sizeof(lvl));

                std::ostringstream info;
                info << entry.path().filename().string() << " | " << name << " (Level " << lvl << ")";
                saveInfo.push_back(info.str());
            }
        }
    }
    return saveInfo;
}

//show full saved characters list
void GameState::listSavedCharacters() {
    std::cout << "\nSaved Characters:\n";
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".dat") {
            std::ifstream inFile(entry.path(), std::ios::binary);
            if (inFile) {
                size_t nameLength;
                inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                std::string name(nameLength, '\0');
                inFile.read(&name[0], nameLength);

                int lvl;
                inFile.read(reinterpret_cast<char*>(&lvl), sizeof(lvl));

                std::cout << " - " << entry.path().filename().string()
                          << " | Name: " << name << " | Level: " << lvl << "\n";
            }
        }
    }
    std::cout << "\n";
}
void GameState::decreaseGold(int amount) { //purchase buffs from Merchant
    gold -= amount;
    if (gold < 0) gold = 0;
}

//Global external function to display stats anywhere
void displayStats() {
    currentGame.displayStats();
}
