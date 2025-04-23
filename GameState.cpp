#include "GameState.h"
#include <iostream>
#include "Header/menuSystem.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
extern GameState currentGame; //Global GameState
void displayStats() { //display player stats on screen
    currentGame.displayStats();
}
GameState::GameState()
    : playerName("Unknown"), level(1), xp(0), xpToNextLevel(45), playerHealth(100),
      attack(10), defense(0), gold(0), playerSneak(3) {}


void GameState::saveToBinary(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open file for saving.\n";
        return;
    }
    // Save basic player info
    size_t nameLength = playerName.size();
    outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    outFile.write(playerName.c_str(), nameLength);
    outFile.write(reinterpret_cast<const char*>(&level), sizeof(level));
    outFile.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    outFile.write(reinterpret_cast<const char*>(&gold), sizeof(gold));

    // Save inventory
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

// Load data from a binary file
bool GameState::loadFromBinary(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open file for loading.\n";
        return false;
    }

    // Read base stats
    size_t nameLength;
    inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    playerName.resize(nameLength);
    inFile.read(&playerName[0], nameLength);
    inFile.read(reinterpret_cast<char*>(&level), sizeof(level));
    inFile.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    inFile.read(reinterpret_cast<char*>(&gold), sizeof(gold));

    // Read inventory
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
    xp = 0;
    xpToNextLevel = 45;

    std::cout << "Game loaded successfully!\n";
    return true;
}

// List .dat save files with player name and level
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

// Level up player stats and auto-save
void GameState::levelUp()
{
    while (xp >= xpToNextLevel) {
        xp -= xpToNextLevel;
        level++;
        xpToNextLevel += level * 30; //scale XP requirement
        playerHealth += level * 20; //increase HP on level up
        attack += level * 10; //boost attack power
        std::cout << "Level up! Now Level " << level << ", HP: " << playerHealth
                  << ", Gold: " << gold << "\n";
    }
    saveToBinary("savegame_" + playerName + ".dat"); //save game after levelUp
}
void GameState::gainXP(int amount) {
    xp += amount;
    std::cout << "You gained " << amount << " XP! Total XP: " << xp << "\n";
}

void GameState::healHP(int amount) {
    playerHealth += amount;
    if (playerHealth > 100) playerHealth = 100;

    std::cout << "You healed for " << amount << " HP! Current HP: " << playerHealth << std::endl;
}

// Let user input their name
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

// Display all character stats
void GameState::displayStats() const {
    std::cout << "=== Character Stats ===\n";
    std::cout << "Name: " << playerName << "\n";
    std::cout << "Level: " << level << " | Health: " << playerHealth << " | Gold: " << gold << "\n";
    std::cout << "XP: " << xp << " / " << xpToNextLevel << "\n";
    std::cout << "Attack: " << attack << " | Defense: " << defense << "\n";
    std::cout << "Inventory:\n";
    for (const auto& item : inventory) {
        std::cout << " - " << item.name << " (" << item.description << ")\n";
    }
}

// Print quick summary of each save file
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