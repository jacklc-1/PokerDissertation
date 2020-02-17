#include "aiclass.h"
#include <iostream>
#include <iomanip>      // std::setprecision
#include <random>
#include <map>


int aiClass::getSuit(int card) const {
    if (card <= 13)
        return 1;
    if (card <= 26)
        return 2;
    if (card <= 39)
        return 3;
    else
        return 4;
}


int aiClass::getNumber(int card) const {
    // Make ace 14
    if (((card - 1) % 13 + 1) == 1) {
        return 14;
    }
    else {

        return (card - 1) % 13 + 1;
    }
}

// Returns the a number corresponding to the play the ai will make
// Fold = 0, call/check = 1, trap call = 2, raise/fold = 3, raise w/ intention to go all in = 4
int aiClass::preFlop()
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 3);

    if (button == true) {
        if (handWeight > 0.9) {
            if (dis(gen) == 1) {
                // Trap call
                return 2;
            }
            else {
                // Raise
                return 4;
            }
        }
        else if (handWeight >= 0.75)
        {
            // Raise
            return 4;
        }
        else if (handWeight > 0.46) {
            // Raise
            return 3;
        }
        else {
            // Fold
            return 0;
        }
    }
    else {
        if (handWeight > 0.9) {
            if (dis(gen) == 1) {
                // Trap Call
                return 2;
            }
            else {
                // Raise
                return 4;
            }
        }
        else if (handWeight >= 0.75)
        {
            // Raise
            return 4;
        }
        else if (handWeight > 0.46) {
            // call/check
            return 1;
        }
        else {
            // Fold
            return 0;
        }

    }
}

int aiClass::pairRank(vector<int> cards) {

    sort(cards.begin(), cards.end());
    if (getNumber(cardOne) == getNumber(cards[cards.size() - 1]) || getNumber(cardTwo) == getNumber(cards[cards.size() - 1]) || getNumber(cardOne) == getNumber(cardTwo)) {
        return 4;
    } else {
        return 2;
    }
}

// Chooses which action for the AI to take

int aiClass::postFlop(vector<int> cards) {

    // Randomization for bluffing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 4);

    // Use this method to figure out if the AI currently has the best hand possible
    nuttedHand(cards);
    cards.push_back(cardOne);
    cards.push_back(cardTwo);

    int cardRankOne = straightFlush(cards);
    int cardRankTwo = quadFullPair(cards);
    int handStrength;

    // Works out the hand strength of AI
    if (cardRankOne > cardRankTwo) {
        handStrength = cardRankOne;
    } else {
        handStrength = cardRankTwo;
    }

    // Chooses the action for the AI to take
    if (handStrength == theNuts) {
        if (dis(gen) == 4) {
            return 4;
        } else {
            return 3;
    } } else if (handStrength >= 2) {
        return 4;
    } else if (handStrength == 1) {
         return pairRank(cards);
    } else if (handStrength == 0) {
        if (dis(gen) == 4) {
            return 4;
        } else {
            return 0;
       }
    }
}

// Works out if the AI has Quads, three of a kind, full house, two pair or pair

int aiClass::quadFullPair(vector<int> cards) {

    // Create map to count number of repeating elements in card vector
    map<int, size_t> count;

    for (int i = 0; i < static_cast<int>(cards.size()); i++) {
        count[getNumber(cards[i])]++;
    }

    // Iterate through map count
    for (auto &e : count) {
        // If AI has four of a kind return 7
        if (e.second == 4) {
            return 7;
        // If AI has three of a kind, check for pair, if pair return 6 (full house), if no pair return 3 (three of a kind)
        } if (e.second == 3) {
            for (auto &d : count) {
                if (d.second == 2) {
                    return 6;
                }
            }
            return 3;
        }
        // If AI has a pair, check for another pair, if two pair return 2, if not return 1
        if (e.second == 2) {
            for (auto &d : count) {
                if (d.second == 2 && e.first != d.first) {
                    return 2;
                }
            }
            return 1;
        }
    }

    return 0;

}

// Works out if the AI has a flush, straight or straight flush

int aiClass::straightFlush(vector<int> cards) {


    bool flushBool = flush(cards);
    bool straightBool = straight(cards);

    if (!flushBool && !straightBool) return 0;
    if (flushBool && straightBool) return 8;
    if (flushBool) return 5;
    if (straightBool) return 4;

}


bool aiClass::flush(vector<int> cards) {

    map<int, size_t> count;

    for (int i = 0; i < static_cast<int>(cards.size()); i++) {
        count[getSuit(cards[i])]++;
    }


    for (auto &e : count) {
        if (e.second == 5) return true;
    }

    return false;
}


bool aiClass::straight(vector<int> cards) {

    for (int i = 0; i < static_cast<int>(cards.size()); i++){
        cards[i] = getNumber(cards[i]);
    }

    cards.erase(unique(cards.begin(), cards.end()), cards.end());
    sort(cards.begin(), cards.end());

    for (int i = 0; i < static_cast<int>(cards.size()) - 4; i++) {
        if (cards[i] == cards[i + 1] - 1 && cards[i] == cards[i + 2] - 2 && cards[i] == cards[i + 3] - 3 && cards[i] == cards[i + 4] - 4) {
            return true;
        }

    }
        return false;
}

// Function which works out the best possible hand given the community cards

void aiClass::nuttedHand(vector<int> cards) {

    sort(cards.begin(), cards.end());

    // When checking for straights, duplicate elements are not considered, therefore we can remove them
    vector <int> straightCards = cards;
    for (int i = 0; i < static_cast<int>(straightCards.size()); i++){
        straightCards[i] = getNumber(straightCards[i]);
    }
    straightCards.erase(unique(straightCards.begin(), straightCards.end()), straightCards.end());

    // Create local variables
    map<int, size_t> countNumber;
    map<int, size_t> countSuit;
    bool pair = false;
    bool three = false;
    bool straight = false;
    bool flush = false;
    bool fourFlush = false;


    // Initialise maps for suits and numbers of cards
    for (int i = 0; i < static_cast<int>(cards.size()); i++) {
        countNumber[getNumber(cards[i])]++;
    }
    for (int i = 0; i < static_cast<int>(cards.size()); i++) {
        countSuit[getSuit(cards[i])]++;
    }

    // Checking for quad/fullhouse possibilites
    for (auto &e : countNumber) {
        if (e.second == 3) {
            three = true;
        }
        if (e.second == 2) {
            pair = true;
        }
     }


    // Checking for straights
    if (straightCards.size() == 3) {
        if (straightCards[1] - straightCards[0] <= 2 && straightCards[2] - straightCards[0] <= 4) {
            straight = true;
        }
    }
    else if (straightCards.size() == 4) {
        if(straightCards[1] - straightCards[0] <= 2 && straightCards[2] - straightCards[0] <= 3
                || straightCards[2] - straightCards[1] <= 2 && straightCards[3] - straightCards[1] <= 3) {
            straight = true;
        }
    }

    //Checking for flush
    for (auto &e : countSuit) {
        if (e.second == 3) {
            flush = true;
        }
        if (e.second == 4) {
            fourFlush = true;
        }
    }

    if (three) {
        theNuts = 7;
    } else if (pair) {
        theNuts = 6;
    } else if (flush) {
        theNuts = 5;
    } else if (straight) {
        theNuts = 4;
    } else {
        theNuts = 3;
    }

}

// Find weight of hand (pre flop) from array[][] of hand weightings

void aiClass::handWeightMethod()
{
    int cardOneLocation = getNumber(cardOne);
    int cardTwoLocation = getNumber(cardTwo);

    if (suited == false) {
        if (cardOneLocation > cardTwoLocation) {
            handWeight = pairWeights[cardTwoLocation - 2][cardOneLocation - 2];

        }
        else if (cardOneLocation == cardTwoLocation) {
            handWeight = pairWeights[cardOneLocation - 2][cardTwoLocation - 2];

        }
        else {
            handWeight = pairWeights[cardOneLocation - 2][cardTwoLocation - 2];
        }

    }
    else {
        if (cardOneLocation > cardTwoLocation) {
            handWeight = pairWeights[cardOneLocation - 2][cardTwoLocation - 2];
        }
        else {
            handWeight = pairWeights[cardTwoLocation - 2][cardOneLocation - 2];
        }
    }
}
