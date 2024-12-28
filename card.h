// card.h
// Edited By: Yuxin Zeng
// Date: 11/29/2024
// Purpose: holds the declarations for functions of the Card class


#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <string>
#include <ctime> 
#include "termfuncs.h"

using namespace std;

class Card {
public:
    // Constructors
    Card(); 

    // Getters
    string getSushiType();
    int getMakiCount();
    
    // Setters
    void setSushiType(string sushiType);
    void setMakiCount(int makiCount);
    
    // Print function
    
private:
    string sushiType;
    int makiCount;
};

#endif
