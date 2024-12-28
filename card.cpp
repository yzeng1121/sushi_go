// card.cpp
// Edited By: Yuxin Zeng
// Date: 11/29/2024
// Purpose: holds the definitions for functions of the Card class


#include "card.h"

// Constructor
Card::Card() {
    sushiType = "";
    makiCount = 0;
}

// Getters -------------------------------------------------------------------
/* getSushiType
 * Input: none
 * Description: allows client to have access to Card's sushi type
 * Output: returns Card's sushi type
 */
string Card::getSushiType() {
    return sushiType;
}

/* getMakiCount
 * Input: none
 * Description: allows client to have access to Card's maki count
 * Output: returns Card's maki count
 */
int Card::getMakiCount() {
    return makiCount;
}


// Setters -------------------------------------------------------------------
/* setSushiType
 * Input: string representing a sushi type
 * Description: allows client to modify Card's sushi type
 * Output: none
 */
void Card::setSushiType(string sushiType) {
    this->sushiType = sushiType;
}

/* setMakiCount
 * Input: int representing Card's maki count
 * Description: allows client to modify Card's maki count
 * Output: none
 */
void Card::setMakiCount(int makiCount) {
    this->makiCount = makiCount;
}
