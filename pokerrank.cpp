#include "pokerrank.h"
#include "holdemgame.h"
using namespace std;

pokerRank::pokerRank(int *passCards, int *passCommCards) {

    for (int i = 0; i < 2; i++) {
        cards.push_back(getNumber(passCards[i]));
        suitCards.push_back(getSuit(passCards[i]));
        full7Cards.push_back(getNumber(passCards[i]));
        passFull7.push_back(getNumber(passCards[i]));
        passSuitCards.push_back(getSuit(passCards[i]));
    }
    for (int i = 0; i < 5; i++) {
        commCards.push_back(getNumber(passCommCards[i]));
        suitCards.push_back(getSuit(passCommCards[i]));
        full7Cards.push_back(getNumber(passCommCards[i]));
        passFull7.push_back(getNumber(passCommCards[i]));
        passSuitCards.push_back(getSuit(passCommCards[i]));

    }

    sort(commCards.begin(), commCards.end());
    sort(full7Cards.begin(), full7Cards.end());
    sort(suitCards.begin(), suitCards.end());

}


int pokerRank::getSuit(int card) const {
    if (card <= 13)
        return 1;
    if (card <= 26)
        return 2;
    if (card <= 39)
        return 3;
    else
        return 4;
}


int pokerRank::getNumber(int card) const {
    // Make ace 14
    if (((card - 1) % 13 + 1) == 1) {
            return 14;
        } else {

    return (card-1) % 13 + 1;   
    }
}

bool pokerRank::mHighCard() {
    bestHandVector = {full7Cards[6],full7Cards[5],full7Cards[4],full7Cards[3],full7Cards[2]};
    return true;
}
bool pokerRank::mOnePair() {


    if (cards[0] == cards[1]) {
        bestHandVector = { cards[0], cards[1], commCards[4], commCards[3], commCards[2] };
        return true;
    }
    bool onePair = false;
    for (int j = 0; j < 6; j++) {

        for (int i = j+1; i < 7; i++) {

            if (full7Cards[j] == full7Cards[i]) {

                bestHandVector = { full7Cards[j], full7Cards[j] };
                onePair = true;
            }
        }
    }

    int i = 6;
    if (onePair == false) {
        return false;
    }else {
        while (bestHandVector.size() < 5) {
            if (bestHandVector[0] != full7Cards[i]) {
                bestHandVector.push_back(full7Cards[i]);
            }
            i--;
        }
    } return true;
}


bool pokerRank::mTwoPair() {


    map<int, size_t> count;
    vector<int> pairs;

    for (int i = 0; i < 7; i++) {
        count[full7Cards[i]]++;

    }

    int highCard = 0;
    // Iterate through map, if there is there are two elements which repeat two and three times, it is a full house
    for (auto &e : count) {
        if (e.second == 2) {
            pairs.push_back(e.first);
        }
        if (e.second != 2 && highCard < e.first) {
            highCard = e.first;
        }
    }


    if (pairs.size() < 2) {
        return false;
    }
    else if (pairs.size() == 2) {
        bestHandVector = { pairs[0], pairs[0], pairs[1], pairs[1], highCard };
        return true;
    }
    else if (pairs.size() > 2) {
        sort(pairs.begin(), pairs.end());
        bestHandVector = { pairs[pairs.size() - 1], pairs[pairs.size() - 1], pairs[pairs.size() - 2] , pairs[pairs.size() - 2], highCard };
        return true;
    }

}

bool pokerRank::mThreeOf(vector<int> full7CardsPass) {

    map<int, size_t> count;

    for (int i = 0; i < 7; i++) {
        count[full7CardsPass[i]]++;

    }

    int three = 0;
    // Iterate through map, if there is there are two elements which repeat two and three times, it is a full house
    for (auto &e : count) {
        if (e.second == 3) {
            three = e.first;
        }
    }

    if (three == 0) {
        return false;
    }
    else {

        for (int i = 0; i < full7CardsPass.size(); i++) {
            if (full7CardsPass[i] == three) {
                full7CardsPass.erase(full7CardsPass.begin() + i);
                i--;
            }
        }
        bestHandVector = { three, three, three, full7CardsPass[full7CardsPass.size() - 1], full7CardsPass[full7CardsPass.size() - 2] };
        return true;
    }
}

bool pokerRank::mStraight(vector<int> straightCards) {


    straightCards.erase(unique(straightCards.begin(), straightCards.end()), straightCards.end());
    sort(straightCards.begin(), straightCards.end());


    for (int i = 0; i < straightCards.size() - 4; i++) {
        if (straightCards[i] == straightCards[i + 1] - 1 && straightCards[i] == straightCards[i + 2] - 2 && straightCards[i] == straightCards[i + 3] - 3 && straightCards[i] == straightCards[i + 4] - 4) {
            bestHandVector = { straightCards[i + 4] };
            return true;
        }

    }

    if (straightCards[0] == 2 && straightCards[1] == 3 && straightCards[2] == 4 && straightCards[3] == 5 && straightCards[straightCards.size() - 1] == 14) {
        bestHandVector = { 5 };
        return true;

    }
    return false;

}

bool pokerRank::mFlush() {



    if (flush7.size() >= 5) {
        for (int i = flush7.size() - 1; bestHandVector.size() < 5; i--) {
            bestHandVector.push_back(flush7[i]);
        }
        return true;
    }
    else {
        return false;
    }



}

bool pokerRank::mFullHouse() {

    // Create map to count number of repeating elements in card vector
    map<int, size_t> count;
    int pair = 0, three = 0;

    for (int i = 0; i < 7; i++) {
        count[full7Cards[i]]++;

    }

    // Iterate through map, if there is there are two elements which repeat two and three times, it is a full house
    for (auto &e : count) {
        if (e.second == 2) {
            pair = e.first;
        } if (e.second == 3) {
            // Special case if there are two three of a kinds on board, choose the higher full house
            if (three != 0) {
                pair = three;
                three = e.first;
                continue;
            }
            three = e.first;
        }
    }

    // If there is a full house, add to map with details
    if (three != 0 && pair != 0) {
        bestHandVector.push_back(three);
        bestHandVector.push_back(pair);
        return true;
    } else {
        return false;
    }
}


bool pokerRank::mQuads() {


    // Create map to count number of repeating elements in card vector
    map<int, size_t> count;
    for (int i = 0; i < 7; i++) {
        count[full7Cards[i]]++;
    }


    for (auto &e : count) {
        if (e.second == 4) {
            if (e.first == full7Cards[6]) {
                bestHandVector = { e.first, full7Cards[5] };
                return true;
            }
            else {
                bestHandVector = { e.first, full7Cards[6] };
                return true;
            }
        }
    }

    return false;
}

bool pokerRank::mStraightFlush() {

    int h = 0, s = 0, c = 0, d = 0;

    for (int i = 0; i < passSuitCards.size(); i++) {
        if (passSuitCards[i] == 1)
            s++;
        if (passSuitCards[i] == 2)
            h++;
        if (passSuitCards[i] == 3)
            c++;
        if (passSuitCards[i] == 4)
            d++;
    }

    if (s >= 5) {
        for (int i = 0; i < passSuitCards.size(); i++) {
            if (passSuitCards[i] != 1) {
                passSuitCards.erase(passSuitCards.begin() + i);
                passFull7.erase(passFull7.begin() + i);

            }
        }
    }
    else if (h >= 5) {
        for (int i = 0; i < passSuitCards.size(); i++) {
            if (passSuitCards[i] != 2) {
                passSuitCards.erase(passSuitCards.begin() + i);
                passFull7.erase(passFull7.begin() + i);

            }
        }
    }
    else if (c >= 5) {
        for (int i = 0; i < passSuitCards.size(); i++) {
            if (passSuitCards[i] != 3) {
                passSuitCards.erase(passSuitCards.begin() + i);
                passFull7.erase(passFull7.begin() + i);

            }
        }
    }
    else if (d >= 5) {
        for (int i = 0; i < passSuitCards.size(); i++) {
            if (passSuitCards[i] != 4) {
                passSuitCards.erase(passSuitCards.begin() + i);
                passFull7.erase(passFull7.begin() + i);

            }
        }
    }
    else {
        return false;
    }

    flush7 = passFull7;

    std::sort(passFull7.begin(), passFull7.end());


    int i = 0;

    for (i; i < passFull7.size() - 4; i++) {
        if (passFull7[i] == passFull7[i + 1] - 1 && passFull7[i] == passFull7[i + 2] - 2 && passFull7[i] == passFull7[i + 3] - 3 && passFull7[i] == passFull7[i + 4] - 4) {
            bestHandVector = { passFull7[i + 4] };
            return true;
        }
        if (passFull7[i] == 2 && passFull7[i + 1] == 3 && passFull7[i + 2] == 4 && passFull7[i + 3] == 5 && passFull7[i + 4] == 14) {
            bestHandVector = { 5 };
            return true;

        }
    }

        return false;
}




int pokerRank::EvaluateHand()
{
    if (mStraightFlush())
        return 8;
    else if (mQuads())
        return 7;
    else if (mFullHouse())
        return 6;
    else if (mFlush())
        return 5;
    else if (mStraight(full7Cards))
        return 4;
    else if (mThreeOf(full7Cards))
        return 3;
    else if (mTwoPair())
        return 2;
    else if (mOnePair())
        return 1;
    else if (mHighCard())
        return 0;
}

