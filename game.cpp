// game.cpp
// Edited By: Yuxin Zeng
// Date: 11/29/2024
// Purpose: holds the definitions for functions of the Game class

#include "game.h"

//constructor
Game::Game(string filename, string play_chopsticks) {
    if (play_chopsticks == "true") { // include chopsticks or not
        this->play_chopsticks = true;
    } else {
        this->play_chopsticks = false;
    }  
    ifstream deckFile(filename); // read from input file to initialize deck
    if (deckFile.fail()) { // error check
        cerr << "Failed to open deck file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    string sushiType; int makiCount;
    deckFile >> sushiType >> sushiType; // skipping the first line
    while (!deckFile.eof()) { // adds each card input into the deck
        Card *card = new Card();
        deckFile >> sushiType;
        card->setSushiType(sushiType);
        if (sushiType == "Maki") { // stores maki count if maki card
            deckFile >> makiCount;
            card->setMakiCount(makiCount);
        }
        deck.push_back(card);
    }
    delete deck.at(deck.size() - 1); // delete duplicate to free up memory
    deck.pop_back(); // get rid of last card duplicate
    if (deck.size() < 81) { // error check
        cerr << "ERROR: Not enough cards to play!" << endl;
        exit(EXIT_FAILURE);
    }
    deckFile.close(); // close the file
}


/* playGame
 * Input: none
 * Description: executes the entire sushigo game
 * Output: no output, prints out text and cards to terminal
 */
void Game::playGame() {
    string play_on;

    for (int round = 0; round < ROUNDS; round++) { // play three rounds
        dealCards(); // dealing each player 9 cards

        selectAndPass();  // selecting & passing cards between the players

        updateScores(); // updates scores for each player
    
        board.drawScore(players); // displays player scores

        if (round < 2) { // prompt to go on to next round
            cout << " End of round! Ready for Round "
                 << round + 2 << " ? (y/n): ";
            cin >> play_on;
            if (play_on != "y") {
                resetHand(); // since skips resetHand() at end of for-loop
                break;
            }
        }                                   
        resetHand(); // resets hand for next round
    }
    countPuddingPoints(); // pudding points counted at end of 3 rounds
    int winner = determineWinner();

    board.drawWinner(players, winner); 
}


/* resetHand
 * Input: none
 * Description: empties the revealedCards and passingHand of each player to
 *              free up memory at the end of each round
 * Output: none
 */
void Game::resetHand() {
    for (int p = 0; p < PLAYER_COUNT; p++) {
        // empties revealedCards of each player
        for (int i = players[p].getRevealedCards()->size() - 1; i > -1; i--) {
            delete players[p].getRevealedCards()->at(i);
            players[p].getRevealedCards()->pop_back();
        } 

        // empties passingHand of each player
        for (int i = players[p].getPassingHand()->size() - 1; i > -1; i--) {
            delete players[p].getPassingHand()->at(i);
            players[p].getPassingHand()->pop_back();
        }
    }
}


/* dealCards
 * Input: none
 * Description: deals 9 cards from deck to each player's passing hand
 * Output: none
 */
void Game::dealCards() {
    Card *lastCard;

    // deals 9 cards to each player
    for (int i = 0; i < CARD_HAND * PLAYER_COUNT; i++) {
        lastCard = deck.back();
        deck.pop_back();

        // gets rid of chopsticks
        if (lastCard->getSushiType() == "Chopsticks") {
            delete lastCard;
            lastCard = deck.back();
            deck.pop_back();
        }

        // adds each individual card to each player's passing hand
        players[i % 3].getPassingHand()->push_back(lastCard);
    }
}


/* selectAndPass
 * Input: none
 * Description: prompts users to choose a card from their passing hand to reveal 
 *              and keep before passing the rest to the player on their right
 * Output: none, prints text to terminal
 */
void Game::selectAndPass() {
    Card *selectedCards[PLAYER_COUNT];
    Vector *passingHand;
    int card_index;

    for (int card = 0; card < CARD_HAND; card++) {
        for (int player = 0; player < PLAYER_COUNT; player++) {
            board.drawBoard(players, player);
            cout << " Player " << player + 1 << ", select a card: ";
            cin >> card_index;
            if (!cin) { // bail out of game if they enter a non-number
                cerr << " INVALID INPUT: No game for you!\n";
                exit(EXIT_FAILURE);
            } 
            passingHand = players[player].getPassingHand();
            while (card_index < 1 || card_index > passingHand->size()) {
                cout << "     Please enter a valid number between 1 ";
                cout << "and " << passingHand->size() << ": ";
                cin >> card_index;
            } // error check
            selectedCards[player] = passingHand->at(card_index - 1);
            passingHand->erase(card_index - 1);
        }
        for (int p = 0; p < PLAYER_COUNT; p++) { // reveal selected cards
            players[p].getRevealedCards()->push_back(selectedCards[p]);
        }
        passRight(); // pass each player's deck to player on right
    }    
}


/* passRight
 * Input: none
 * Description: shuffles each player's passing hand to the player on their right
 * Output: none
 */
void Game::passRight() {
    Vector *tempHand = players[0].getPassingHand();
    players[0].setPassingHand(players[2].getPassingHand());
    players[2].setPassingHand(players[1].getPassingHand());
    players[1].setPassingHand(tempHand);
}


/* updateScores
 * Input: none
 * Description: updates each player's score based on each sushi type and updates
 *              each player's pudding count based on quantity of pudding cards
 *              in their hand
 * Output: none
 */
void Game::updateScores() {
    Vector *revealedCards;
    
    // counts points based off all sushi types in players' revealedHand
    countMakiPoints();
    countTempuraPoints();
    countSashimiPoints();
    countDumplingPoints();
    countNigiriPoints();


    // searches, counts, & stores pudding amount
    for (int p = 0; p < PLAYER_COUNT; p++) {
        revealedCards = players[p].getRevealedCards();
        for (int c = 0; c < CARD_HAND; c++) {
            if (revealedCards->at(c)->getSushiType() == "Pudding") {
                players[p].setPuddingCount(players[p].getPuddingCount() + 1);
            }
        }
    }
}


/* countMakiPoints
 * Input: none
 * Description: counts the number of maki each player has and allocates
 *              scores to each player accordingly
 * Output: none
 */
void Game::countMakiPoints() {
    int makiCount[PLAYER_COUNT] = {0, 0, 0};
    string type; int count;

    for (int p = 0; p < PLAYER_COUNT; p++) { // counts + stores maki per player
        for (int c = 0; c < CARD_HAND; c++) {
            type = players[p].getRevealedCards()->at(c)->getSushiType();
            if (type == "Maki") { // stores maki count
                count = players[p].getRevealedCards()->at(c)->getMakiCount();
                makiCount[p] += count;
            }
        }
    }

    // allocates appropriate points from maki count
    findMakiMost(makiCount); 
}


/* findMakiMost
 * Input: integer array representing the amount of maki each player has
 * Description: finds the player(s) with the most (& second most) maki and deals 
 *              the appropriate amount of points to those players
 * Output: none
 */
void Game::findMakiMost(int makiCount[]) {
    int gold = -1, silver = gold, bronze = silver;
    int firstP = 0, secondP = firstP, thirdP = secondP;
    for (int p = 0; p < PLAYER_COUNT; p++) { // searches + ranks maki count
        if (makiCount[p] > gold) { 
            bronze = silver; silver = gold; gold = makiCount[p];
            thirdP = secondP; secondP = firstP; firstP = p;
        } else if (makiCount[p] > silver) {
            bronze = silver; silver = makiCount[p];
            thirdP = secondP; secondP = p;
        } else {
            bronze = makiCount[p]; thirdP = p;
        }
    }

    if (gold > silver && silver > bronze) { // single winner
        players[firstP].setScore(players[firstP].getScore() + 6);
        players[secondP].setScore(players[secondP].getScore() + 3);
    } else if (gold == silver && silver > bronze) { // tie first
        players[firstP].setScore(players[firstP].getScore() + 3);
        players[secondP].setScore(players[secondP].getScore() + 3);
    } else if (gold > silver && silver == bronze) { // tie second
        players[firstP].setScore(players[firstP].getScore() + 6);
        players[secondP].setScore(players[secondP].getScore() + 1);
        players[thirdP].setScore(players[thirdP].getScore() + 1);
    } else if (gold == bronze) { // tie all
        players[firstP].setScore(players[firstP].getScore() + 2);
        players[secondP].setScore(players[secondP].getScore() + 2);
        players[thirdP].setScore(players[thirdP].getScore() + 2);
    }
}


/* countTempuraPoints
 * Input: none
 * Description: counts the number of tempura each player has and allocates
 *              scores to each player accordingly
 * Output: none
 */
void Game::countTempuraPoints() {
    int tempuraCount[PLAYER_COUNT] = {0, 0, 0};
    string type; int score;

    for (int p = 0; p < PLAYER_COUNT; p++) { // counts + stores tempura
        for (int c = 0; c < CARD_HAND; c++) {
            type = players[p].getRevealedCards()->at(c)->getSushiType();
            if (type == "Tempura") {
                tempuraCount[p] += 1;
                if (tempuraCount[p] > 1) { // adds points for tempura pairs
                    score = players[p].getScore();
                    players[p].setScore(score + 5);
                    tempuraCount[p] -= 2;
                }
            }
        }
    }
}


/* countSashimiPoints
 * Input: none
 * Description: counts the number of sashimi each player has and allocates
 *              scores to each player accordingly
 * Output: none
 */
void Game::countSashimiPoints() {
    int sashimiCount[PLAYER_COUNT] = {0, 0, 0};
    string type; int score;

    for (int p = 0; p < PLAYER_COUNT; p++) { // counts + stores sashimi
        for (int c = 0; c < CARD_HAND; c++) {
            type = players[p].getRevealedCards()->at(c)->getSushiType();
            if (type == "Sashimi") {
                sashimiCount[p] += 1;
                if (sashimiCount[p] > 2) { // adds points for sashimi triplets
                    score = players[p].getScore();
                    players[p].setScore(score + 10);
                    sashimiCount[p] -= 3;
                }
            }
        }
    }
}


/* countDumplingPoints
 * Input: none
 * Description: counts the number of dumplings each player has and allocates
 *              scores to each player accordingly
 * Output: none
 */
void Game::countDumplingPoints() {
    int dumplingCount[PLAYER_COUNT] = {0, 0, 0};
    string type; int score;

    for (int p = 0; p < PLAYER_COUNT; p++) { // counts + stores dumpling
        for (int c = 0; c < CARD_HAND; c++) {
            type = players[p].getRevealedCards()->at(c)->getSushiType();
            if (type == "Dumpling") {
                dumplingCount[p] += 1;
            }
        }
    }

    // allocates points for amount of dumplings
    for (int p = 0; p < PLAYER_COUNT; p++) {
        score = players[p].getScore();
        if (dumplingCount[p] >= 5) {
            players[p].setScore(score + 15);
        } else if (dumplingCount[p] == 4) {
            players[p].setScore(score + 10);
        } else if (dumplingCount[p] == 3) {
            players[p].setScore(score + 6);
        } else if (dumplingCount[p] == 2) {
            players[p].setScore(score + 3);
        } else if (dumplingCount[p] == 1) {
            players[p].setScore(score + 1);
        }
    }
}


/* countNigiriPoints
 * Input: none
 * Description: counts the number of nigiri each player has and allocates
 *              scores to each player accordingly based on nigiri types
 * Output: none
 */
void Game::countNigiriPoints() {
    string type;
    
    for (int p = 0; p < PLAYER_COUNT; p++) {
        for (int c = 0; c < CARD_HAND; c++) {
            type = players[p].getRevealedCards()->at(c)->getSushiType();
            if (type == "Egg-Nigiri") {
                if (checkWasabi(p, players[p].getRevealedCards()->at(c))) {
                    players[p].setScore(players[p].getScore() + 3);
                } else { // no eligible wasabi in hand
                    players[p].setScore(players[p].getScore() + 1);
                }
            } else if (type == "Salmon-Nigiri") {
                if (checkWasabi(p, players[p].getRevealedCards()->at(c))) {
                    players[p].setScore(players[p].getScore() + 6);
                } else { // no eligible wasabi in hand
                    players[p].setScore(players[p].getScore() + 2);
                }
            } else if (type == "Squid-Nigiri") {
                if (checkWasabi(p, players[p].getRevealedCards()->at(c))) {
                    players[p].setScore(players[p].getScore() + 9);
                } else { // no eligible wasabi in hand
                    players[p].setScore(players[p].getScore() + 3);
                }
            }
        }
    }
}


/* checkWasabi
 * Input: int representing player & pointer to a nigiri card
 * Description: checks whether player has eligible & unused wasabi in their 
 *              hand, allowing them to triple their nigiri points
 * Output: bool representing whether player has an eligible & unused wasabi
 */
bool Game::checkWasabi(int player, Card *nigiri) {
    string type;
    string egg = "Egg-Nigiri", salmon = "Salmon-Nigiri", squid = "Squid-Nigiri";
    int wasabiCount = 0, c = 0;
    Card *current = players[player].getRevealedCards()->at(c);

    while (current != nigiri) { 
        type = current->getSushiType();
        if (type == egg || type == salmon || type == squid) {
            if (wasabiCount > 0) {
                wasabiCount -= 1;
            } 
        } else if (type == "Wasabi") {
            wasabiCount += 1;
        }
        current = players[player].getRevealedCards()->at(c += 1);
    }

    if (wasabiCount > 0) {
        return true; 
    }
    return false;
}


/* countPuddingPoints
 * Input: none
 * Description: counts the number of pudding each player has and allocates
 *              scores to each player accordingly
 * Output: none
 */
void Game::countPuddingPoints() {
    int gold = -1, silver = gold, bronze = silver, puddingCount;
    int firstP = 0, secondP = 0, thirdP = 0;

    for (int p = 0; p < PLAYER_COUNT; p++) { // ranks pudding count
        puddingCount = players[p].getPuddingCount();
        if (puddingCount > gold) { 
            bronze = silver; silver = gold; gold = puddingCount;
            thirdP = secondP; secondP = firstP; firstP = p;
        } else if (puddingCount > silver) {
            bronze = silver; silver = puddingCount;
            thirdP = secondP; secondP = p;
        } else {
            bronze = puddingCount;
            thirdP = p;
        }
    }

    if (gold == silver && silver > bronze) { // tie first
        players[firstP].setScore(players[firstP].getScore() + 3);
        players[secondP].setScore(players[secondP].getScore() + 3);
        players[thirdP].setScore(players[thirdP].getScore() - 6);
    } else if (gold > silver && silver == bronze) { // tie last
        players[firstP].setScore(players[firstP].getScore() + 6);
        players[secondP].setScore(players[secondP].getScore() - 3);
        players[thirdP].setScore(players[thirdP].getScore() - 3);
    } else if (gold > silver) { // most & least, no ties
        players[firstP].setScore(players[firstP].getScore() + 6);
        players[thirdP].setScore(players[thirdP].getScore() - 6);
    } 
}


/* determineWinner
 * Input: none
 * Description: determines the outcome of the game: single winner or tie
 * Output: int representing player who won or if game has tied winners
 */
int Game::determineWinner() {
    int gold = 0, silver = gold, bronze = silver;
    int firstP = 0, secondP = firstP, thirdP = secondP;
    for (int p = 0; p < PLAYER_COUNT; p++) { // ranks points
        if (players[p].getScore() > gold) {
            bronze = silver; silver = gold; gold = players[p].getScore();
            thirdP = secondP; secondP = firstP; firstP = p;
        } else if (players[p].getScore() > silver) {
            bronze = silver; silver = players[p].getScore();
            secondP = p;
        } else {
            bronze = players[p].getScore();
            thirdP = p;
        }
    }

    if (gold > silver) { // one winner
        return firstP;
    } else if (gold == silver) { // 1+ first place based off points
        return puddingWinner();
    }
    return -1;
}


/* puddingWinner
 * Input: none
 * Description: in the case of multiple players tying for most points, 
 *              determines amount of pudding cards each player has as a tie
 *              breaker
 * Output: int representing player who won or if game has tied winners
 */
int Game::puddingWinner() {
    int gold = 0, silver = gold;
    int firstP = 0, secondP = firstP;
    int score1 = 0, score2 = 0;

    for (int p = 0; p < PLAYER_COUNT; p++) { // ranks pudding count
        if (players[p].getPuddingCount() > gold) { 
            silver = gold; gold = players[p].getPuddingCount();
            secondP = firstP; firstP = p;
        } else if (players[p].getPuddingCount() > silver) {
            silver = players[p].getPuddingCount(); secondP = p; 
        }
    }

    score1 = players[firstP].getScore();
    score2 = players[secondP].getScore();
    if (gold > silver && score1 >= score2) {
        return firstP;
    }
    return -1;
}


// Destructor
Game::~Game() {
    for (int i = 0; i < deck.size(); i++) {
        delete deck.at(i);
    }
}
