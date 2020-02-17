#ifndef HOLDEMGAME_H
#define HOLDEMGAME_H


#include <QMainWindow>
#include "pokerrank.h"
#include <aiclass.h>


namespace Ui {
class holdEmGame;
}

class holdEmGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit holdEmGame(QWidget *parent = nullptr, QString name = nullptr);
    ~holdEmGame();

    void dealCards();
    QString getCardImagePath(int cardNumber);
    void buttonGen();
    void buttonChange();
    void blinds();
    void playerAction();
    void checkOrCall();
    void flopMethod();
    void turnMethod();
    void riverMethod();
    void changeStack();
    void showDown();
    void delay(int n);
    void playerWins();
    void aiWins();
    void chopPot();
    void hideCommCards();
    void allIn();
    void gameEnds();
    void streetFunction();

    void aiClassPre(int n);
    void aiClassPost(int n);
    void aiClassCall();
    void aiClassRaise();
    void aiClassFold();
    void aiClassCheck();

    QString playerName;

private slots:
    void on_foldButton_clicked();

    void on_checkCallButton_clicked();

    void on_betSlider_sliderMoved(int position);

    void on_customBetText_textChanged(const QString &arg1);

    void on_betButton_clicked();

    void on_pushButton_clicked();

    void on_betSlider_sliderPressed();

private:
    Ui::holdEmGame *ui;
    // Cards 1 and 2 for both player and AI
    int aiCard1;
    int aiCard2;
    int pCard1;
    int pCard2;
    int button;
    int pStackSize = 5000;
    int aiStackSize = 5000;
    int startingStack = 5000;
    int ante = 10;
    int smallBlind = 50;
    int bigBlind = 100;
    bool opponentsTurn;
    int totalPot;
    int pre, flop, turn,river;
    std::vector<int> cards;

    // aiClass vars
    int preFlopAction;
    int postFlopAction;
    bool playerRaised = false;
    bool playerAllIn = false;
    bool aiRaised = false;
    bool aiAllIn = false;

};

#endif // HOLDEMGAME_H
