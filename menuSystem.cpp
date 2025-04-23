#include "Header/menuSystem.h"
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

// Clears the screen
void clearScreen() {
    system("cls");  // Windows clear screen
}

// Displays a universal menu system
int displayMenu(const std::vector<std::string>& options, const std::string& header) {
    int selectedOption = 0;

    while (true) {
        clearScreen();
        //raw string for game title graphic
        std::cout << R"(
    ________________________________________________________
        ~*~      F I N A L   A D V E N T U R E !     ~*~
    ________________________________________________________
   (Use UP / DOWN Arrow keys & ENTER to select. ESC to exit.)
)" << "\n";
        if (!header.empty()) {  //displays menu header under the title
            std::cout << header << "\n\n";
        }
        // loop through options and print with cursor
        for (size_t i = 0; i < options.size(); i++) {
            std::cout << (i == selectedOption ? "  > " : "    ") << options[i] << "\n";
        }
        switch (_getch()) { //read arrow keys, enter key, Esc key
        case 224: { //Arrow keys (2 codes: initial ASCII 224, Up=72, Down=80)
                int key = _getch(); // get second code
                if (key == 72) selectedOption = (selectedOption - 1 + options.size()) % options.size(); // Up
                if (key == 80) selectedOption = (selectedOption + 1) % options.size(); // Down
                break;
        }
        case 13: //ENTER key (ASCII 13)
            return selectedOption; // returns selected index
        case 27: //ESC key (ASCII 27)
            std::cout << "\nExiting game...\n";
            cin.ignore();
            exit(0); // terminate program
        }
    }
}

