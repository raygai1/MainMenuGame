#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>

void clearScreen();
int displayMenu(const std::vector<std::string>& options, const std::string& header = "");

#endif
