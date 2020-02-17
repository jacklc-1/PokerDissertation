#ifndef AICLASS_H
#define AICLASS_H
#include <vector>

using namespace std;

class aiClass {

public:

    aiClass(int aiCard1, int aiCard2, int button) : cardOne(aiCard1),cardTwo(aiCard2) {

        if (button == 1) this->button = false; else this->button = true;

        if (getSuit(aiCard1) == getSuit(aiCard2)) suited = true; else  suited = false;
        handWeightMethod();
    }

    aiClass(int aiCard1, int aiCard2) : cardOne(aiCard1),cardTwo(aiCard2){}

    int getSuit(int card) const;
    int getNumber(int number) const;
    int preFlop();
    void handWeightMethod();
    int postFlop(vector<int> cards);
    void nuttedHand(vector<int> cards);
    int pairRank(vector<int> cards);
    int quadFullPair(vector<int> cards);
    int straightFlush(vector<int> cards);
    bool straight(vector<int> cards);
    bool flush(vector<int> cards);



private:
    int cardOne, cardTwo;
    bool suited;
    bool button;
    double handWeight;
    int theNuts;
    vector<int> handPossible;
    double pairWeights[13][13] = { {0.63, 0.39, 0.40, 0.41, 0.41, 0.42, 0.44, 0.47, 0.49, 0.53, 0.56, 0.60, 0.65},
                                {0.44, 0.66, 0.43, 0.43, 0.44, 0.45, 0.45, 0.48, 0.50, 0.54, 0.58, 0.62, 0.66},
                                {0.44, 0.47, 0.69, 0.46, 0.46, 0.46, 0.47, 0.48, 0.51, 0.55, 0.59, 0.62, 0.67},
                                {0.47, 0.48, 0.50, 0.73, 0.48, 0.49, 0.49, 0.51, 0.53, 0.56, 0.60, 0.63, 0.69},
                                {0.45, 0.47, 0.49, 0.51, 0.76, 0.50, 0.51, 0.53, 0.55, 0.57, 0.61, 0.64, 0.68},
                                {0.46, 0.48, 0.49, 0.52, 0.54, 0.79, 0.54, 0.55, 0.57, 0.60, 0.61, 0.66, 0.70},
                                {0.48, 0.48, 0.51, 0.53, 0.54, 0.58, 0.82, 0.57, 0.59, 0.61, 0.63, 0.67, 0.71},
                                {0.52, 0.51, 0.52, 0.54, 0.56, 0.59, 0.60, 0.85, 0.61, 0.63, 0.65, 0.68, 0.72},
                                {0.53, 0.55, 0.57, 0.57, 0.58, 0.59, 0.62, 0.64, 0.88, 0.66, 0.68, 0.71, 0.75},
                                {0.57, 0.57, 0.58, 0.60, 0.60, 0.62, 0.63, 0.66, 0.69, 0.93, 0.69, 0.72, 0.75},
                                {0.60, 0.61, 0.63, 0.63, 0.64, 0.64, 0.66, 0.68, 0.70, 0.72, 0.93, 0.73, 0.76},
                                {0.63, 0.65, 0.66, 0.66, 0.67, 0.68, 0.69, 0.71, 0.74, 0.73, 0.75, 0.97, 0.78},
                                {0.68, 0.70, 0.70, 0.70, 0.72, 0.72, 0.72, 0.75, 0.76, 0.78, 0.78, 0.81, 1.00} };

};

#endif // AICLASS_H

