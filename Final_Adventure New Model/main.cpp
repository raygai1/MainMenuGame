#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "Header/GameState.h"
#include "Header/menuSystem.h"
#include "Header/combatSystem.h"

using namespace std;
namespace fs = std::filesystem;

GameState currentGame; //tracks current player state globally

void startGame(GameState& game);
void loadGame(GameState& game);
void showMenu(GameState& game);
void choosePathAndFight(GameState& game, int luck);

int main() {
    GameState game; // new game instance
    showMenu(game);
    return 0;
}
//main menu loop
void showMenu(GameState& game) {
    vector<string> menuOptions = {
        "Start New Game",
        "Load Game",
        "Exit"
    };

    while (true) {
        int choice = displayMenu(menuOptions);

        switch (choice) {
        case 0:
            startGame(game);
            break;
        case 1:
            loadGame(game);
            break;
        case 2:
            cout << "Exiting game. See you next time!\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// starts a new game with default values
void startGame(GameState& game) {
    clearScreen();
    cout << "Starting a new game...\n";

    if (game.playerName == "Unknown") {
        game.setPlayerName(); // prompt for character name
        game.saveToBinary("savegame_" + game.playerName + ".dat");
    }
    cout << "Welcome, " << game.playerName << "! Let's begin your adventure...\n";
    cin.ignore();
    choosePathAndFight(game, game.level);
}
//loads selected save file and resumes game
void loadGame(GameState& game) {
    clearScreen();
    vector<string> saves = game.getSaveFilesInfo();

    if (saves.empty()) {
        cout << "No save files found.\n";
        cout << "\nPress any key to return to the menu...";
        _getch();
        return;
    }
    cout << "Select a save file:\n";
    int choice = displayMenu(saves);
    // extract filename from line
    string selectedLine = saves[choice];
    string filename = selectedLine.substr(0, selectedLine.find(" "));

    if (game.loadFromBinary(filename)) {
        cout << "Game loaded! Welcome back, " << game.playerName << "!\n";
        cin.ignore();
        choosePathAndFight(game, game.level);
    } else {
        cout << "Failed to load game.\n";
        cout << "\nPress any key to return to the menu...";
        _getch();
    }
}
