#include "GameState.h"        //player data structure
#include "menuSystem.h"       //menu options
#include "Items.h"            //item definitions
#include <iostream>           //print output
#include <fstream>            //read/write saves
#include <filesystem>         //filesystem utilities
using namespace std;
namespace fs = filesystem;
extern GameState currentGame; //Global GameState object
int enemiesDefeated = 0;      //tracks enemies for boss fight

//get current gold amount
int GameState::getGold() const {
    return gold;
}

//reduce gold when purchasing buffs
void GameState::decreaseGold(int amount) {
    gold -= amount;
    if (gold < 0) gold = 0; //no negatives
}

//get max health (same as playerHealth here)
int GameState::getMaxHealth() const {
    return playerHealth;
}

//boost max health
void GameState::increaseMaxHealth(int amount) {
    playerHealth += amount;
}

//get attack value
int GameState::getAttack() const {
    return attack;
}

//boost attack value
void GameState::increaseAttack(int amount) {
    attack += amount;
}

//get sneak skill value
int GameState::getSneak() const {
    return playerSneak;
}

//boost sneak skill
void GameState::increaseSneak(int amount) {
    playerSneak += amount;
}

//boost run away bonus
void GameState::increaseRunBonus(int amount) {
    runBonus += amount;
}

//add new item and apply stat effect
void GameState::applyItem(const Item& item) {
    inventory.push_back(item); //store item
    switch (item.type) {
    case ItemType::Weapon:
        attack += item.statEffect; //+atk
        break;
    case ItemType::Armor:
        defense += item.statEffect; //+def
        break;
    case ItemType::CursedWeapon:
        attack += item.statEffect; //-atk
        break;
    case ItemType::CursedArmor:
        defense += item.statEffect; //-def
        break;
    default:
        break; //Funny items = 0
    }
}

//call display stats globally
void displayStats() {
    currentGame.displayStats();
}

//default constructor sets starting stats
GameState::GameState()
    : playerName("Unknown"), level(1), xp(0), xpToNextLevel(45), playerHealth(100),
      attack(10), defense(0), gold(0), playerSneak(3) {}

//save current player to a .dat file
void GameState::saveToBinary(const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Failed to open file for saving.\n";
        return;
    }

    //save player info
    size_t nameLength = playerName.size();
    outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    outFile.write(playerName.c_str(), nameLength);
    outFile.write(reinterpret_cast<const char*>(&level), sizeof(level));
    outFile.write(reinterpret_cast<const char*>(&xp), sizeof(xp));
    outFile.write(reinterpret_cast<const char*>(&xpToNextLevel), sizeof(xpToNextLevel));
    outFile.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
    outFile.write(reinterpret_cast<const char*>(&attack), sizeof(attack));
    outFile.write(reinterpret_cast<const char*>(&defense), sizeof(defense));
    outFile.write(reinterpret_cast<const char*>(&gold), sizeof(gold));
    outFile.write(reinterpret_cast<const char*>(&playerSneak), sizeof(playerSneak));
    outFile.write(reinterpret_cast<const char*>(&runBonus), sizeof(runBonus));
    outFile.write(reinterpret_cast<const char*>(&enemiesDefeated), sizeof(enemiesDefeated));
    outFile.write(reinterpret_cast<const char*>(&bossDefeated), sizeof(bossDefeated));

    //save inventory items
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

    cout << "Game saved successfully!\n";
}

//load player data from .dat file
bool GameState::loadFromBinary(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Failed to open file for loading.\n";
        return false;
    }

    //read player basics
    size_t nameLength;
    inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    playerName.resize(nameLength);
    inFile.read(&playerName[0], nameLength);
    inFile.read(reinterpret_cast<char*>(&level), sizeof(level));
    inFile.read(reinterpret_cast<char*>(&playerHealth), sizeof(playerHealth));
    inFile.read(reinterpret_cast<char*>(&gold), sizeof(gold));

    //load inventory
    size_t inventorySize;
    inFile.read(reinterpret_cast<char*>(&inventorySize), sizeof(inventorySize));
    inventory.clear();
    for (size_t i = 0; i < inventorySize; ++i) {
        size_t nameLen, descLen;
        inFile.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(nameLen, '\0');
        inFile.read(&name[0], nameLen);
        inFile.read(reinterpret_cast<char*>(&descLen), sizeof(descLen));
        string desc(descLen, '\0');
        inFile.read(&desc[0], descLen);
        int typeInt;
        inFile.read(reinterpret_cast<char*>(&typeInt), sizeof(typeInt));
        int effect;
        inFile.read(reinterpret_cast<char*>(&effect), sizeof(effect));
        Item item(name, desc, static_cast<ItemType>(typeInt), effect);
        applyItem(item); //apply item stat boost
    }

    //rebuild stats after loading
    xp = 0;
    xpToNextLevel = 45 + level * 30;
    attack = 10 + (level - 1) * 10;
    playerHealth = 100 + (level - 1) * 20;
    defense = 0;
    playerSneak = level * 3;

    cout << "Loaded stats - HP: " << playerHealth << ", Atk: " << attack << ", Def: " << defense << "\n";
    cout << "Game loaded successfully!\n";
    return true;
}

//show list of saved games
vector<string> GameState::getSaveFilesInfo() {
    vector<string> saveInfo;
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".dat") {
            ifstream inFile(entry.path(), ios::binary);
            if (inFile) {
                size_t nameLength;
                inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                string name(nameLength, '\0');
                inFile.read(&name[0], nameLength);
                int lvl;
                inFile.read(reinterpret_cast<char*>(&lvl), sizeof(lvl));

                ostringstream info;
                info << entry.path().filename().string() << " | " << name << " (Level " << lvl << ")";
                saveInfo.push_back(info.str());
            }
        }
    }
    return saveInfo;
}

//level up player and stats
void GameState::levelUp() {
    while (xp >= xpToNextLevel) {
        xp -= xpToNextLevel;
        level++;
        xpToNextLevel += level * 30;
        playerHealth += level * 20;
        attack += level * 10;
        cout << "Level up! Now Level " << level << ", HP: " << playerHealth
                  << ", Gold: " << gold << "\n";
    }
    saveToBinary("savegame_" + playerName + ".dat"); //save after level up
}

//add xp points
void GameState::gainXP(int amount) {
    xp += amount;
    cout << "You gained " << amount << " XP! Total XP: " << xp << "\n";
}

//add gold
void GameState::addGold(int amount) {
    gold += amount;
    cout << "You gained " << amount << " gold!\n";
}

//heal player hp
void GameState::healHP(int amount) {
    playerHealth += amount;
    if (playerHealth > 100) playerHealth = 100;
    cout << "You healed for " << amount << " HP! Current HP: " << playerHealth << endl;
}

//ask player for character name
void GameState::setPlayerName() {
    cout << "Enter your character's name (letters/spaces only): ";
    getline(cin, playerName);

    for (char& c : playerName) {
        if (!isalpha(c) && !isspace(c)) {
            cout << "Invalid name. Using default: Unknown\n";
            playerName = "Unknown";
            return;
        }
    }
    if (playerName.empty()) playerName = "Unknown";
}

//display all player stats nicely
void GameState::displayStats() const {
    cout << "=== Character Stats ===\n";
    cout << "Name: " << playerName << "\n";
    cout << "Level: " << level << " | Health: " << playerHealth << " | Gold: " << gold << "\n";
    cout << "XP: " << xp << " / " << xpToNextLevel << "\n";
    cout << "Attack: " << attack << " | Defense: " << defense << "\n";
    cout << "Inventory:\n";
    for (const auto& item : inventory) {
        cout << " - " << item.name << " (" << item.description << ")\n";
    }
}

//show a summary of save files
void GameState::listSavedCharacters() {
    cout << "\nSaved Characters:\n";
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".dat") {
            ifstream inFile(entry.path(), ios::binary);
            if (inFile) {
                size_t nameLength;
                inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                string name(nameLength, '\0');
                inFile.read(&name[0], nameLength);
                int lvl;
                inFile.read(reinterpret_cast<char*>(&lvl), sizeof(lvl));
                cout << " - " << entry.path().filename().string()
                          << " | Name: " << name << " | Level: " << lvl << "\n";
            }
        }
    }
    cout << "\n";
}
