cmake_minimum_required(VERSION 3.30)
project(Final_Adventure)

set(CMAKE_CXX_STANDARD 20)

# Add all necessary source files
add_executable(MainMenuGame
        combatSystem.cpp
        GameState.cpp
        main.cpp
        Paths.cpp
        menuSystem.cpp
        Header/combatSystem.h
        Header/Environment.h
        Header/GameState.h
        Header/Enemy.h
        Header/menuSystem.h
        Header/Items.h
        Items.cpp
        Header/Game_Header.h)

# Include directories for header files
include_directories(Header)
