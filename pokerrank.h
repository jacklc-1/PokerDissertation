#ifndef POKERRANK_H
#define POKERRANK_H
#include <QString>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;


class pokerRank {
public:

    pokerRank(int *passCards, int *passCommCards);
    int getSuit(int card) const;
    int getNumber(int number) const;


    bool mHighCard(), mOnePair(),mTwoPair(), mThreeOf(vector<int> full7CardsPass), mStraight(vector<int> straightCards), mFlush(), mFullHouse(), mQuads(), mStraightFlush(), mRoyalFlush();


   int EvaluateHand();
   vector<int> bestHandVector;


protected:

    map<int,int> suitsAndCards;
    vector<int> cards;
    vector<int> commCards;
    vector<int> suitCards;
    vector<int> full7Cards;
    vector<int> highestFlush;
    vector<int> passSuitCards;
    vector<int> passFull7;
    vector<int> flush7;


};

#endif // POKERRANK_H

