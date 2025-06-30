// who: Devin McCaw (dmccaw, A03351745) 
// what: This program is meant to be an Odium's Mansion game. A user
// will traverse throughout an 8x8 grid attempting to pick up hidden
// treasure. The explorer must avoid several traps whle exploring.
// why:  Project 2: Odium's Mansion
// when: 04/27/2025

#include <iostream>
#include <ctime>
#include "ui.h"

int main(int argc, char const *argv[])
{
    srand(time(0));
    GameUI game;
    game.start();
    std::cout << "Thanks for playing!\nEnter any key to exit...";
    std::cin.get(); // Wait for user input before exiting
    return 0;
}


