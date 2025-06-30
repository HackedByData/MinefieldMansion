#include <iostream>
#include <vector>
#include <ctime>
#include "ui.h"
#include "odiums_mansion.h"

uint32_t userPos{};

GameUI::GameUI()
{
    userPos = 0; // Starting position of the user
    do {evilAldracPos = rand() % (ROW_QTY * COL_QTY);}
    while ((evilAldracPos == 0) ||
    (evilAldracPos == 1) ||
    (evilAldracPos == ROW_QTY) ||
    (evilAldracPos == pit1.pitPos) ||
    (evilAldracPos == pit2.pitPos) ||
    (evilAldracPos == pit3.pitPos)); //Random position for evil Aldric, not (0,0 or 0,1 or 1,0)
    do {goldPos = rand() % (ROW_QTY * COL_QTY);}
    while ((goldPos == 0) ||
    (goldPos == 1) ||
    (goldPos == ROW_QTY) ||
    (goldPos == pit1.pitPos) ||
    (goldPos == pit2.pitPos) ||
    (goldPos == pit3.pitPos));
} // Random position for gold, not (0,0 or 0,1 or 1,0) and not on a pit or aldrac

GameUI::~GameUI()
{
    
    userPos = 0; // Reset user position
    evilAldracPos = 0; // Reset evil Aldrac position
    goldPos = 0; // Reset gold position
    pit1.pitPos = 0; // Reset pit 1 position
    pit2.pitPos = 0; // Reset pit 2 position
    pit3.pitPos = 0; // Reset pit 3 position
    for (uint64_t i = 0; i < ROW_QTY * COL_QTY; ++i)
    {
        history[i] = VisitedHistory(); // Reset history
    }
}

GameUI::Pit::Pit()
{
    do(pitPos = rand() % (ROW_QTY * COL_QTY)); // Random position for pit
    while (pitPos == 0 || pitPos == 1 || pitPos == ROW_QTY); // can't be at (0,0 or 0,1 or 1,0)
}

void GameUI::start()
{

    welcomeMsg();

    while(true)
    {
        char move;
        printEnvironment();
        if (isDead) 
        {
            std::cout << "YOU DIED.\n";
            break;
        }
        if (hasGold && userPos == 0)
        {
            std::cout << "You escaped with the gold! You win!\n";
            break;
        }
        if (hasGold) std::cout << "You are holding the gold.\n";
        move = getValidMove();

        if(move == EXIT) 
        {
            std::cout << "Quitting game...";
            
            break;
        }
        processMove(move);
        std::cout << std::endl << std::endl;
        checkSurroundings(userPos);
    }   

   
}
void GameUI::processMove(char move)
{
    switch (move)
    {
    case LEFT:
        --userPos;
        break;
    case RIGHT:
        ++userPos;
        break;
    case UP:
        userPos -= COL_QTY;
        break;
    case DOWN:
        userPos += COL_QTY;
        break;
    case PICKUPGOLD:
        if (userPos == goldPos && !hasGold)
        {
            std::cout << "You picked up the gold!\n";
            hasGold = true;
        }
        else if (hasGold)
        {
            std::cout << "You already have the gold in your clutches. Escape!!";
        }
        else
        {
            std::cout << "There is no gold here.\n";
        }
    }   
}

void GameUI::checkSurroundings(uint32_t userPos)
{
    //check for Evil Aldrac
    // Check for Evil Aldrac
    if ((!testIfUp(userPos) && userPos - COL_QTY == evilAldracPos) ||
        (!testIfDown(userPos) && userPos + COL_QTY == evilAldracPos) ||
        (!testIfLeft(userPos) && userPos - 1 == evilAldracPos) ||
        (!testIfRight(userPos) && userPos + 1 == evilAldracPos))
        {
            history[userPos].setStench();
            std::cout << "You smell a foul odor nearby...\n";
        }

    std::vector<uint32_t> pitPositions = { pit1.pitPos, pit2.pitPos, pit3.pitPos };
    for (uint32_t pit : pitPositions) {
        if ((!testIfUp(userPos) && userPos - COL_QTY == pit) ||
            (!testIfDown(userPos) && userPos + COL_QTY == pit) ||
            (!testIfLeft(userPos) && userPos - 1 == pit) ||
            (!testIfRight(userPos) && userPos + 1 == pit))
            {
                history[userPos].setBreezy();
                std::cout << "You feel a slight breeze in the room.\n";
            }
    }
    if (userPos == goldPos)
    {
        history[userPos].setGlimmer();
        std::cout << "You see something glimmering...\n";
        
    }
    else if (userPos == evilAldracPos)
    {
        std::cout << "The evil Aldrac, cursed be his name, caught you sneaking through his room and ate you for dinner.\n";
        isDead = true;
        
    }
    for (uint32_t pit : pitPositions)
    {
        if (userPos == pit)
        {
            std::cout << "You trip and fall down a bottomless pit. AAHHHHHHHHH!!!!\n";
            isDead = true;
        }
    }
        

}

bool GameUI::testIfRight(uint64_t userPos)
{
    if (userPos % COL_QTY == (COL_QTY - 1)) return true;
    return false;
}

bool GameUI::testIfLeft(uint64_t userPos)
{
    if (userPos % COL_QTY == 0) return true;
    return false;
}

bool GameUI::testIfUp(uint64_t userPos)
{
    if (userPos-8 > COL_QTY * ROW_QTY) return true;
    return false;
}

bool GameUI::testIfDown(uint64_t userPos)
{
    if (userPos + COL_QTY >= ROW_QTY * COL_QTY) return true;
    return false;
}

void GameUI::welcomeMsg()
{
    std::cout << "Aldrac Dungeon Simulator\nAvoid at all costs the pits and the evil Aldrac, cursed be his name.\n";
}

void GameUI::printEnvironment()
{
    for (uint64_t i = 0; i < ROW_QTY; ++i)
    {
       printLine(); 
       printRow(i);
    }
    printLine(); 
    
}

char GameUI::getValidMove()
{
    char input;    
    std::cout << "Enter your move\n(R)ight, (L)eft, (U)p, (D)own, (P)ickup gold, (Q)uit:";
    std::cin >> input;
    clearInstream();
    input = toupper(input);
    if (input != LEFT && input != RIGHT
        && input != UP &&
        input != DOWN &&
        input != PICKUPGOLD &&
        input != EXIT)
    {
        std::cout << "Invalid move. Please try again.\n";
        return getValidMove();
    }
    if ((input == UP && testIfUp(userPos)) ||
    (input == LEFT && testIfLeft(userPos)) ||
    (input == DOWN && testIfDown(userPos)) ||
    (input == RIGHT && testIfRight(userPos)))
    {
        std::cout << "Invalid direction. Please try again. \n";
        return getValidMove();
    }
    return input;
}

void GameUI::printLine()
{
    for (uint64_t i = 0; i < COL_QTY; ++i)
        std::cout << "------";
    std::cout << "-\n";
}

void GameUI::printRow(uint64_t row)
{
    uint64_t pos;
    char symbol;
    for (uint64_t i = 0; i < COL_QTY; ++i)
    {
        pos = row * ROW_QTY + i;
        symbol = (history[pos].isStench()) ?  STENCH : ' ';
        std::cout <<"| " << symbol;
        symbol = (history[pos].isGlimmer() && !hasGold) ? GLIMMER : ' ';
        std::cout << symbol;
        symbol = (history[pos].isBreezy()) ? BREEZY : ' ';
        std::cout << symbol << " "; // Check for breezy
    }
    std::cout << "|\n";
    for (uint64_t i = 0; i < COL_QTY; ++i)
    {
        pos = row * ROW_QTY + i;
        symbol = (pos == userPos) ? USER_SYMBOL : ' ';
        std::cout << "|  " << symbol << "  ";
    }
    std::cout << "|\n";
}

void clearScreen()
{
    if(WINDOWS)
        system("CLS");
    else
        system("clear");
}

void clearInstream()
{
    std::cin.clear();
    // Wrong input remains on the stream, so you need to get rid of it
    std::cin.ignore(INT_MAX, '\n');
}

bool GameUI::VisitedHistory::isStench()
{
    return stench;
}

bool GameUI::VisitedHistory::isGlimmer()
{
    return glimmer;
}

bool GameUI::VisitedHistory::isBreezy()
{
    return breezy;
}

void GameUI::VisitedHistory::setStench()
{
    stench = true;
}

void GameUI::VisitedHistory::setGlimmer()
{
    glimmer = true;
}

void GameUI::VisitedHistory::setBreezy()
{
    breezy = true;
}
