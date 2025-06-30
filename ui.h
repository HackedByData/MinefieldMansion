#include <cstdint>

#ifndef UI_H
#define UI_H
#define ROW_QTY 8
#define COL_QTY 8
#define USER_SYMBOL 'U'
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'
#define PICKUPGOLD 'P'
#define EXIT 'Q'
#define STENCH 'S'
#define BREEZY 'B'
#define GLIMMER 'G'

void clearScreen();
void clearInstream();

class GameUI
{
    public:
        GameUI();
        ~GameUI();
        void start();
    private:
        void welcomeMsg();
        void printEnvironment();
        char getValidMove();
        void processMove(char);
        void checkSurroundings(uint32_t);
        struct Pit
        {
            Pit();
            uint32_t pitPos{};
            bool isGold{false};
            bool isEvilAldrac{false};
        };
        Pit pit1, pit2, pit3;
        class VisitedHistory
        {
            public:
                bool isStench();
                bool isGlimmer();
                bool isBreezy();
                void setStench();
                void setGlimmer();
                void setBreezy();
            private:
                bool stench = false;
                bool glimmer = false;
                bool breezy = false;
            
        };
        bool testIfRight(uint64_t);
        bool testIfLeft(uint64_t);
        bool testIfUp(uint64_t);
        bool testIfDown(uint64_t);
        bool hasGold = false;
        bool isDead = false;
        void printLine();
        void printRow(uint64_t);
        VisitedHistory history[ROW_QTY * COL_QTY];
        uint32_t userPos{};
        uint32_t evilAldracPos{};
        uint32_t goldPos{};


};

#endif