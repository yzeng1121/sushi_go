// player.h
// Author: Yuxin Zeng (yzeng06)
// Date Started: 11/21/24
// Purpose: defines Player object and declares its member variables & functions


#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"

class Player {
public:
    Player();
    ~Player(); 

    // Getters
    Vector *getPassingHand();
    Vector *getRevealedCards();
    int getScore();
    int getPuddingCount();

    // Setters
    void setPassingHand(Vector *passingHand);
    void setRevealedCards(Vector *revealedCards);
    void setScore(int score);
    void setPuddingCount(int puddingCount);

private:
    Vector *passingHand;
    Vector *revealedCards;
    int score;
    int puddingCount;
};

#endif