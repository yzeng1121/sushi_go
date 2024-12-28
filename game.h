// game.h
// Edited By: Yuxin Zeng
// Date: 11/29/2024
// Purpose: holds the declarations for functions of the Game class

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "board.h"
#include "card.h"
#include "player.h"

using namespace std;

class Game {
    public:
        // constructor/destructor
        Game(string filename, string play_chopsticks);
        ~Game();

        // main gameplay
        void playGame();
        
    private:
        // helper functions
        void dealCards();
        void selectAndPass();
        void updateScores();
        void passRight();
        void resetHand();
        int determineWinner();
        int puddingWinner(); 

        // score counting functions 
        void countMakiPoints();
        void findMakiMost(int makiCount[]);
        void countTempuraPoints();
        void countSashimiPoints();
        void countDumplingPoints();
        void countNigiriPoints();
        bool checkWasabi(int player, Card *nigiri);
        void countPuddingPoints();

        // constants
        static const int PLAYER_COUNT = 3;
        static const int ROUNDS = 3;
        static const int CARD_HAND = 9;

        // chopsticks activated true/false
        bool play_chopsticks;

        // card deck
        Vector deck;

        // game objects
        Board board;
        Player players[PLAYER_COUNT];
};

#endif
