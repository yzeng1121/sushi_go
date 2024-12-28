#include <iostream>
#include <string>
#include "game.h"
using namespace std;

int main(int argc, char *argv[]){
    if(argc < 3){
        cerr << "ERROR: Expecting a data filename, and an indicator of ";
        cerr << "whether to play with chopsticks." << endl;
        exit(EXIT_FAILURE);
    }
    
    // Set the screen colors
    screen_bg("black");
    screen_fg("white");

    Game my_game(argv[1], argv[2]);
    my_game.playGame();
    
    // Return the screen colors to normal
    screen_reset();
    return 0;
}
