// player.cpp
// Author: Yuxin Zeng
// Date: 11/29/2024
// Purpose: hold the definitions for functions of the Card class


#include "player.h"

// Constructor
Player::Player() {
    score = 0;
    puddingCount = 0;
    passingHand = new Vector();
    revealedCards = new Vector();
}

// Destructor
Player::~Player() {
    for (int c = 0; c < passingHand->size(); c++) {
        delete passingHand->at(c);
    }
    delete passingHand;
    
    for (int c = 0; c < revealedCards->size(); c++) {
        delete revealedCards->at(c);
    }
    delete revealedCards;
}


// Getters -------------------------------------------------------------------
/* getPassingHand
 * Input: none
 * Description: allows client to have access to Player's passing hand 
 * Output: pointer to Player's passing hand vector
 */
Vector *Player::getPassingHand() {
    return passingHand;
}

/* getRevealedCards
 * Input: none
 * Description: allows client to have access to Player's revealed cards
 * Output: pointer to Player's revealed cards vector
 */
Vector *Player::getRevealedCards() {
    return revealedCards;
}

/* getScore
 * Input: none
 * Description: allows client to have access to Player's score
 * Output: int representing Player's score
 */
int Player::getScore() {
    return score;
}

/* getPuddingCount
 * Input: none
 * Description: allows client to have access to Player's pudding count
 * Output: int representing Player's pudding count
 */
int Player::getPuddingCount() {
    return puddingCount;
}


// Setters -------------------------------------------------------------------
/* setPassingHand
 * Input: pointer to a vector representing passing hand
 * Description: allows client to modify Player's passing hand
 * Output: none
 */
void Player::setPassingHand(Vector *passingHand) {
    this->passingHand = passingHand;
}

/* setRevealedCards
 * Input: pointer to a vector representing revealed cards
 * Description: allows client to modify Player's revealed cards
 * Output: none
 */
void Player::setRevealedCards(Vector *revealedCards) {
    this->revealedCards = revealedCards;
}

/* setScore
 * Input: int representing a score
 * Description: allows client to modify Player's score
 * Output: none
 */
void Player::setScore(int score) {
    this->score = score;
}

/* setPuddingCount
 * Input: int representing pudding count
 * Description: allows client to modify Player's pudding count
 * Output: none
 */
void Player::setPuddingCount(int puddingCount) {
    this->puddingCount = puddingCount;
}   

