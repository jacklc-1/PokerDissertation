#include "holdemgame.h"
#include "ui_holdemgame.h"
#include "pokerrank.h"
#include "aiclass.h"
#include <random>
#include <set>
#include <string>
#include <thread>
#include <chrono>
#include <QTime>
#include <QMessageBox>

using namespace std;
using namespace std::this_thread; // sleep_for
using namespace std::chrono; // seconds

// Random number gen
std::random_device rd;
std::mt19937 gen(rd());

// Constructor
holdEmGame::holdEmGame(QWidget *parent, QString name) :
    QMainWindow(parent), playerName(name),
    ui(new Ui::holdEmGame)
{

    ui->setupUi(this);
    ui->playerName->setText(playerName); // Set name of player entered from previous screen
    // Choose where the button lies, if = 1 it is on player, if 2 then on AI
    ui->customBetText->setPlaceholderText("Minimum: $" + QString::number(2 * bigBlind));
    ui->betButton->hide();
    ui->checkCallButton->hide();
    ui->betSlider->hide();
    ui->foldButton->hide();
    ui->customBetText->hide();

    // Set stack size
    ui->playerStackLabel->setText("$" + QString::number(pStackSize));
    ui->aiStackLabel->setText("$" + QString::number(aiStackSize));

    // Set background image
    QPixmap background(":/cards/cards/Background_pokerTable.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);

    // Set Chip Images and hide them,
    QPixmap chipImage(":/cards/cards/chipImage.png");
    ui->pBetImage->setPixmap(chipImage.scaled(ui->pBetImage->width(), ui->pBetImage->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->aiBetImage->setPixmap(chipImage.scaled(ui->aiBetImage->width(), ui->aiBetImage->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->pBetImage->hide();
    ui->aiBetImage->hide();

    // Set button image
    QPixmap buttonImage(":/cards/cards/dealerButton.png");
    ui->pDealerButton->setPixmap(buttonImage.scaled(ui->playerCard1->width(), ui->playerCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->aiDealerButton->setPixmap(buttonImage.scaled(ui->playerCard1->width(), ui->playerCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->pDealerButton->hide();
    ui->aiDealerButton->hide();

}



// Destructor
holdEmGame::~holdEmGame()
{
    delete ui;
}

// Method to deal the cards
void holdEmGame::dealCards() {



    hideCommCards();
    aiRaised = false; playerRaised = false; aiAllIn = false; playerRaised = false;
    // Create random number distibution between 1 and 52 (card numbers) and create card vector for cards used in the hand
    std::uniform_int_distribution<> dis(1,52);
    pre = 1; turn=0; river=0; totalPot = 0;

    // Calls method to set up round of play

    ui->playerCard1->show();
    ui->playerCard2->show();
    ui->paiCard1->show();
    ui->paiCard2->show();

    blinds();
    playerAction();

    ui->betButton->setText("Bet $" + QString::number(2*bigBlind));

    // Randomly pick 12 cards for the hand, ensuring none are the same:
    // 2 for each player, 5 community cards and 3 "burn" cards (burn cards are discarded before each round of community cards)
    int randNum;
    cards.clear();
    do {
        randNum = dis(gen);

        if (std::find(cards.begin(), cards.end(), randNum) == cards.end())
        {
            cards.push_back(randNum);

        }
    } while (cards.size() < 12);



    // Set players' cards
    if (button == 1) {

        // Show image of players first card
        QString text = getCardImagePath(cards[0]);
        QPixmap pCardPic1(text);
        ui->playerCard1->setPixmap(pCardPic1.scaled(ui->playerCard1->width(), ui->playerCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Show image of players second card
        text = getCardImagePath(cards[2]);
        QPixmap pCardPic2(text);
        ui->playerCard2->setPixmap(pCardPic2.scaled(ui->playerCard2->width(), ui->playerCard2->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Set AI cards to back of cards
        QPixmap playerBackCard(":/cards/cards/Red_back.jpg");
               ui->paiCard1->setPixmap(playerBackCard.scaled(ui->paiCard1->width(), ui->paiCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
               ui->paiCard2->setPixmap(playerBackCard.scaled(ui->paiCard2->width(), ui->paiCard2->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Set the cards of the player/AI
        pCard1 = cards[0];
        pCard2 = cards[2];
        aiCard1 = cards[1];
        aiCard2 = cards[3];

        aiClass *ai = new aiClass(aiCard1, aiCard2, 1);
        preFlopAction = ai->preFlop();
        delete ai;


    } else {

        // Show image of players first card
        QString text = getCardImagePath(cards[1]);
        QPixmap pCardPic1(text);
        ui->playerCard1->setPixmap(pCardPic1.scaled(ui->playerCard1->width(), ui->playerCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Show image of players second card
        text = getCardImagePath(cards[3]);
        QPixmap pCardPic2(text);
        ui->playerCard2->setPixmap(pCardPic2.scaled(ui->playerCard2->width(), ui->playerCard2->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Set AI cards to back of cards
        QPixmap playerBackCard("C:/Users/Jack/Documents/Downloads/cards_jpeg_zip/JPEG/Red_back.jpg");
               ui->paiCard1->setPixmap(playerBackCard.scaled(ui->paiCard1->width(), ui->paiCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
               ui->paiCard2->setPixmap(playerBackCard.scaled(ui->paiCard2->width(), ui->paiCard2->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));



        // Set the cards of the player/AI
        pCard1 = cards[1];
        pCard2 = cards[3];
        aiCard1 = cards[0];
        aiCard2 = cards[2];

        aiClass *ai = new aiClass(aiCard1, aiCard2, 2);
        preFlopAction = ai->preFlop();
        aiClassPre(preFlopAction);
        delete ai;


    }

}

void holdEmGame::streetFunction() {

    if (pre == 1) {
        flopMethod();
    } else if (flop == 1) {
        turnMethod();
    } else if (turn == 1) {
        riverMethod();
    } else if (river == 1) {
        showDown();
    }
}

/********************************/
// Ai class methods

// Fold = 0, call/check = 1, trap call = 2, raise/fold = 3, raise w/ intention to go all in = 4
void holdEmGame::aiClassPre(int n) {

    delay(1);


        if (button == 1) {
            if (n == 0) {
                if (playerRaised) {
                    aiClassFold();
                } else {
                    flopMethod();
                }
            } else if (n == 1 || n == 2) {
                if (playerRaised) {
                    aiClassCall();
                } else {
                    flopMethod();
                }
            } else if (n == 4) {
                aiClassRaise();
            }
        }else {
            if (n == 0) {
                aiClassFold();
            }
            else if (n == 1 || n == 2) {
                if (playerRaised) {
                    aiClassCall();
                } else {
                    flopMethod();
                }
            }
            else if (n == 3 || n == 4) {
                aiClassRaise();
            }

        }

    opponentsTurn = false;
    playerAction();
}

void holdEmGame::aiClassPost(int n) {

    delay(1);


        if (button == 1) {
            if (n == 0) {
                if (playerRaised) {
                    aiClassFold();
                }  opponentsTurn = false;
                playerAction();
            } else if (n == 2) {
                if (playerRaised) {
                    aiClassCall();
                } else {
                    aiClassRaise();
                    opponentsTurn = false;
                    playerAction();
                }
            } else if (n == 3) {
                if (river == 1) {
                    aiClassRaise();
                    opponentsTurn = false;
                    playerAction();
                } else {
                    if (playerRaised) {
                        aiClassCall();
                    }  }
            } else if (n == 4) {
                aiClassRaise();
                opponentsTurn = false;
                playerAction();
            }

        }else {
            if (n == 0) {
                if (playerRaised) {
                    aiClassFold();
                } else {
                    streetFunction();
                }
            }
            else if (n == 2) {
                if (playerRaised) {
                    aiClassCall();
                } else {
                    streetFunction();
                }
            }
            else if (n == 3) {
                if (river == 1) {
                    aiClassRaise();
                } else {
                    if (playerRaised) {
                        aiClassCall();
                    } else {
                        streetFunction();
                    } }
            }
            else if (n == 4) {
                aiClassRaise();
            }

        }



}


void holdEmGame::aiClassCall() {

    ui->aiBetSize->show();
    ui->aiBetImage->show();
    if (button == 1) {
        int aiStackChange = ui->pBetSize->text().split("$")[1].toInt() - ui->aiBetSize->text().split("$")[1].toInt();
        ui->aiBetSize->setText(ui->pBetSize->text());
        ui->aiStackLabel->setText("$" + QString::number(ui->aiStackLabel->text().split("$")[1].toInt() - aiStackChange));
        delay(1);
        totalPot += aiStackChange;
        streetFunction();
    }
    // button on ai
    else {
        int aiStackChange = ui->pBetSize->text().split("$")[1].toInt() - ui->aiBetSize->text().split("$")[1].toInt();
        ui->aiBetSize->setText(ui->pBetSize->text());
        ui->aiStackLabel->setText("$" + QString::number(ui->aiStackLabel->text().split("$")[1].toInt() - aiStackChange));
        totalPot += aiStackChange;
    }
}

void holdEmGame::aiClassRaise() {

    delay(2);

    ui->aiBetSize->show();
    ui->aiBetImage->show();
    if (pre == 1 || playerRaised) {
    if (ui->aiStackLabel->text().split("$")[1].toInt() < (10 * bigBlind) || ui->aiStackLabel->text().split("$")[1].toInt() < (2.5 * ui->pBetSize->text().split("$")[1].toInt())) {
        aiAllIn = true;
        ui->aiBetSize->setText("$" + QString::number(ui->aiBetSize->text().split("$")[1].toInt() + ui->aiStackLabel->text().split("$")[1].toInt()));
        totalPot += ui->aiStackLabel->text().split("$")[1].toInt();
        ui->aiStackLabel->setText("$0");

    } else {
        aiRaised = true;
        int betAmount = ui->pBetSize->text().split("$")[1].toInt() * 2.5;
        int aiStackChange = betAmount -  ui->aiBetSize->text().split("$")[1].toInt();
        ui->aiBetSize->setText("$" + QString::number(betAmount));
        ui->aiStackLabel->setText("$" + QString::number(ui->aiStackLabel->text().split("$")[1].toInt() - aiStackChange));
        totalPot += aiStackChange;

    } } else {
        aiRaised = true;
        int betAmount = totalPot * 0.5;
        int aiStackChange = betAmount -  ui->aiBetSize->text().split("$")[1].toInt();
        ui->aiBetSize->setText("$" + QString::number(betAmount));
        ui->aiStackLabel->setText("$" + QString::number(ui->aiStackLabel->text().split("$")[1].toInt() - aiStackChange));
        totalPot += betAmount;
            }
    checkOrCall();
    ui->betButton->setText("Bet $" + QString::number(2 * (ui->aiBetSize->text().split("$")[1].toInt() - ui->pBetSize->text().split("$")[1].toInt())));
    ui->betButton->show();
    ui->foldButton->show();
    ui->betSlider->show();
    ui->customBetText->show();
    ui->checkCallButton->show();

}

void holdEmGame::aiClassFold() {
    delay(1);
    playerWins();
    buttonChange();
    dealCards();
}



// Method to set the flop cards to the corresponding pictures
void holdEmGame::flopMethod() {


    QString text = getCardImagePath(cards[5]);
    QPixmap commCard1(text);
    ui->communityCard1->setPixmap(commCard1.scaled(ui->communityCard1->width(), ui->communityCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    text = getCardImagePath(cards[6]);
    QPixmap commCard2(text);
    ui->communityCard2->setPixmap(commCard2.scaled(ui->communityCard2->width(), ui->communityCard2->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    text = getCardImagePath(cards[7]);
    QPixmap commCard3(text);
    ui->communityCard3->setPixmap(commCard3.scaled(ui->communityCard3->width(), ui->communityCard3->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pre = 0; flop = 1;
    if (!playerAllIn || !aiAllIn) {
    if (button == 1) {
        opponentsTurn = true;
        playerAction();
    }
    ui->aiBetSize->setText("$0");
    ui->pBetSize->setText("$0");


    ui->totalPotLabel->setText("$" + QString::number(totalPot));

    aiRaised = false;
    playerRaised = false;
    checkOrCall();
    ui->pBetSize->hide();
    ui->aiBetSize->hide();
    ui->pBetImage->hide();
    ui->aiBetImage->hide();

    aiClass *ai = new aiClass(aiCard1, aiCard2);
    vector<int> passCards = {cards[5],cards[6],cards[7]};
    postFlopAction = ai->postFlop(passCards);
    delete ai;

    if (button == 1) {
        aiClassPost(postFlopAction);
    } else {
        opponentsTurn = false;
        playerAction();
   }

}

}

// Method to set the turn card to the corresponding picture
void holdEmGame::turnMethod() {

    QString text = getCardImagePath(cards[9]);
    QPixmap commCard4(text);
    ui->communityCard4->setPixmap(commCard4.scaled(ui->communityCard4->width(), ui->communityCard4->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    flop = 0; turn = 1;
    if (!playerAllIn || !aiAllIn) {
    if (button == 1) {
        opponentsTurn = true;
        playerAction();
    }

    ui->aiBetSize->setText("$0");
    ui->pBetSize->setText("$0");




    aiRaised = false;
    playerRaised = false;
    checkOrCall();
    ui->pBetSize->hide();
    ui->aiBetSize->hide();
    ui->pBetImage->hide();
    ui->aiBetImage->hide();

    ui->totalPotLabel->setText("$" + QString::number(totalPot));
    aiClass *ai = new aiClass(aiCard1, aiCard2);
    vector<int> passCards = {cards[5],cards[6],cards[7],cards[9]};
    postFlopAction = ai->postFlop(passCards);
    delete ai;

    if (button == 1) {
        aiClassPost(postFlopAction);
    } else {
        opponentsTurn = false;
        playerAction();
   }

} }

// Method to set the river card to the corresponding picture
void holdEmGame::riverMethod() {

    turn = 0; river = 1;
    QString text = getCardImagePath(cards[11]);
    QPixmap commCard5(text);
    ui->communityCard5->setPixmap(commCard5.scaled(ui->communityCard5->width(), ui->communityCard5->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    if (!playerAllIn || !aiAllIn) {
    if (button == 1) {
        opponentsTurn = true;
        playerAction();
    }
    ui->aiBetSize->setText("$0");
    ui->pBetSize->setText("$0");


    aiRaised = false;
    playerRaised = false;
    checkOrCall();
    ui->pBetSize->hide();
    ui->aiBetSize->hide();
    ui->pBetImage->hide();
    ui->aiBetImage->hide();
    ui->totalPotLabel->setText("$" + QString::number(totalPot));
    aiClass *ai = new aiClass(aiCard1, aiCard2);
    vector<int> passCards = {cards[5],cards[6],cards[7],cards[9],cards[11]};
    postFlopAction = ai->postFlop(passCards);
    delete ai;

    if (button == 1) {
        aiClassPost(postFlopAction);
    } else {
        opponentsTurn = false;
        playerAction();
   }

} }

// Decides where the button is placed for the first hand
void holdEmGame::buttonGen() {
    std::uniform_int_distribution<> dis(1,2);
    button = dis(gen);
    // Show chip image for blinds
    if (button == 1) {
        opponentsTurn = false;
        ui->pDealerButton->show();
    }else {
        opponentsTurn = true;
        ui->aiDealerButton->show();
    }
}

// Change the player on the button after a hand concludes
void holdEmGame::buttonChange() {
    // If button isn't on player, hide all the option buttons as it's opponents time to act
    if (button == 1) {
        ui->pDealerButton->hide();
        ui->aiDealerButton->show();
        opponentsTurn = true;
        button = 2;
    }
    else {
        ui->pDealerButton->show();
        ui->aiDealerButton->hide();
        button = 1;

    }
}

// Returns a QString, which is a path towards the corresponding card image
QString holdEmGame::getCardImagePath(int cardNumber) {

    return QString(":/cards/cards/") + QString::number(cardNumber) + ".jpg";

}

// Method to set up the blinds pre flop
void holdEmGame::blinds() {

    // Show the chip image of each player
    ui->pBetImage->show();
    ui->aiBetImage->show();
    ui->pBetSize->show();
    ui->aiBetSize->show();
    // Calculate the total pot
   totalPot = smallBlind + bigBlind;
    // Take corresponding blinds from each players stack and set bet size next to chip image
    if (button == 1) {
        pStackSize = ui->playerStackLabel->text().split("$")[1].toInt() - smallBlind;
        aiStackSize = ui->aiStackLabel->text().split("$")[1].toInt() - bigBlind;
        opponentsTurn = false;
        ui->playerStackLabel->setText("$" + QString::number(pStackSize));
        ui->aiStackLabel->setText("$" + QString::number(aiStackSize));
        ui->pBetSize->setText("$" + QString::number(smallBlind));
        ui->aiBetSize->setText("$" + QString::number(bigBlind));

    } else {
        aiStackSize = ui->aiStackLabel->text().split("$")[1].toInt() - smallBlind;
        pStackSize = ui->playerStackLabel->text().split("$")[1].toInt() - bigBlind;
        opponentsTurn = true;
        ui->playerStackLabel->setText("$" + QString::number(pStackSize));
        ui->aiStackLabel->setText("$" + QString::number(aiStackSize));
        ui->pBetSize->setText("$" + QString::number(bigBlind));
        ui->aiBetSize->setText("$" + QString::number(smallBlind));
    }

}

// Check if action is on player, if it is - show option buttons
void holdEmGame::playerAction() {
    if (!opponentsTurn) {
        ui->betButton->setText("Bet $" + QString::number(2 * (ui->aiBetSize->text().split("$")[1].toInt() - ui->pBetSize->text().split("$")[1].toInt())));
        ui->betButton->show();
        ui->foldButton->show();
        ui->betSlider->show();
        ui->customBetText->show();
        ui->checkCallButton->show();
        if (aiAllIn) {
            ui->betButton->hide();
            ui->betSlider->hide();
            ui->customBetText->hide();
        }
        checkOrCall();
    } else {
        ui->betButton->hide();
        ui->foldButton->hide();
        ui->betSlider->hide();
        ui->customBetText->hide();
        ui->checkCallButton->hide();
        ui->checkCallButton->hide();

    }
}

void holdEmGame::changeStack() {


    int changeStack = ui->playerStackLabel->text().split("$")[1].toInt() + ui->pBetSize->text().split("$")[1].toInt();
    totalPot += ui->aiBetSize->text().split("$")[1].toInt() - ui->pBetSize->text().split("$")[1].toInt();
    changeStack -= ui->aiBetSize->text().split("$")[1].toInt();
    ui->playerStackLabel->setText("$" + QString::number(changeStack));
    ui->pBetSize->setText(ui->aiBetSize->text());
    ui->pBetSize->show();
    ui->pBetImage->show();
    delay(1);

}

// Changes the middle option button to check or call, depending on which action is possible
void holdEmGame::checkOrCall() {

    if (pre == 1) {
        if (button == 1) {
            ui->checkCallButton->setText("Call");
        }
        if (aiRaised) {
            ui->checkCallButton->setText("Call");
        }
    } else {
    // If ai's bet is more than our own, set the button to call
    if (aiRaised) {

        ui->checkCallButton->setText("Call");
    }
    // else, set the button to check
    else {

        ui->checkCallButton->setText("Check");
    }
}
}

// Method for when the check/call button is pressed by player
void holdEmGame::on_checkCallButton_clicked()
{
    if (aiAllIn) {
        totalPot += ui->aiBetSize->text().split("$")[1].toInt() - ui->pBetSize->text().split("$")[1].toInt();
        int temp = (ui->aiBetSize->text().split("$")[1].toInt() - ui->pBetSize->text().split("$")[1].toInt());
        int stackChange = ui->playerStackLabel->text().split("$")[1].toInt() - temp;
        ui->playerStackLabel->setText("$" + QString::number(stackChange));
        ui->pBetSize->setText(ui->aiBetSize->text());
        allIn();
    }
    // Preflop
    else if (pre == 1 && !aiAllIn) {
        if (button == 1) {
            opponentsTurn = true;
            changeStack();
            playerAction();
            ui->totalPotLabel->setText("$" + QString::number(totalPot));
            if (!aiRaised) {
                aiClassPre(preFlopAction);
            } else {
                flopMethod();
            }
        } else {
            changeStack();
            flopMethod();
        }
    }
    // Flop
    else if (flop == 1 && !aiAllIn) {
        if (button == 1) {
            if (aiRaised) {
                changeStack();
            }
            turnMethod();   
        } else {
            if (aiRaised) {
                changeStack();
                turnMethod();
            } else {
                opponentsTurn = true;
                playerAction();
                aiClassPost(postFlopAction);
            }
        } 
    }
    // Turn
    else if (turn == 1 && !aiAllIn) {
        
        if (button == 1){
            if (aiRaised) {
                changeStack();
            }

            riverMethod();

        } else {
            if (aiRaised) {
                changeStack();
                riverMethod();
            } else {
                opponentsTurn = true;
                playerAction();
                aiClassPost(postFlopAction);
            }
        } 
    }
    else if (river == 1 && !aiAllIn) {

        if (button == 1){
            if (aiRaised) {
                changeStack();
            }
            showDown();
        } else {
            if (aiRaised) {
                changeStack();
                showDown();
            } else {
                opponentsTurn = true;
                playerAction();
                aiClassPost(postFlopAction);
            }
        }  }
}


// Method for when fold button is pressed by player
void holdEmGame::on_foldButton_clicked()
{
    // Add pot to AI's stack
    aiWins();
    // Change button as new round commencing
    buttonChange();
    dealCards();

}


/*
 *
 * BET BUTTON METHODS
 *
 * */

// Method for changing the bet button sizing when the bet slider is moved
void holdEmGame::on_betSlider_sliderMoved(int position)
{
    ui->betButton->setText("Bet $" + QString::number(position));
}

void holdEmGame::on_betSlider_sliderPressed()
{
    int minumumBet = 2 * (ui->aiBetSize->text().split("$")[1].toInt() - ui->pBetSize->text().split("$")[1].toInt());
    ui->betSlider->setMinimum(minumumBet);
    ui->betSlider->setMaximum(ui->playerStackLabel->text().split("$")[1].toInt());
}



// Method for changing the bet button sizing when viable number is entered in to the bet text box
void holdEmGame::on_customBetText_textChanged(const QString &arg1)
{
    int minumumBet = 2 * (ui->aiBetSize->text().split("$")[1].toInt() - ui->pBetSize->text().split("$")[1].toInt());
    if (minumumBet <= arg1.toInt() && arg1.toInt() <= pStackSize + ui->pBetSize->text().split("$")[1].toInt()) {
        ui->betButton->setText("Bet $" + arg1);
    }

}

// Method for making the bet when bet button is clicked
void holdEmGame::on_betButton_clicked()
{

    playerRaised = true;
    ui->pBetSize->show();
    ui->pBetImage->show();

    if (ui->betButton->text().split("Bet $")[1].toInt() == ui->playerStackLabel->text().split("$")[1].toInt()) {
        playerAllIn = true;
        int betChange = ui->betButton->text().split("Bet $")[1].toInt() + ui->pBetSize->text().split("$")[1].toInt();
        totalPot += ui->betButton->text().split("Bet $")[1].toInt();
        ui->pBetSize->setText("$" + QString::number(betChange));
        ui->playerStackLabel->setText("$" + QString::number(ui->playerStackLabel->text().split("$")[1].toInt() - ui->betButton->text().split("Bet $")[1].toInt()));
        opponentsTurn = true;
        playerAction();
        aiClass *ai = new aiClass(aiCard1, aiCard2);
        vector<int> passCards = {cards[5],cards[6],cards[7],cards[9], cards[11]};
        postFlopAction = ai->postFlop(passCards);
        delete ai;
        if (postFlopAction == 4 || postFlopAction == 3) {
                    delay(2);
                    aiClassCall();
                    allIn();
        } else {
            aiClassFold();
        }

    } else {

    int betChange = ui->betButton->text().split("Bet $")[1].toInt() + ui->pBetSize->text().split("$")[1].toInt();
    totalPot += ui->betButton->text().split("Bet $")[1].toInt();
    ui->pBetSize->setText("$" + QString::number(betChange));
    ui->playerStackLabel->setText("$" + QString::number(ui->playerStackLabel->text().split("$")[1].toInt() - ui->betButton->text().split("Bet $")[1].toInt()));
    opponentsTurn = true;
    playerAction();
    }


      if (pre == 1 && !playerAllIn) {
        aiClassPre(preFlopAction);
    } else if (flop == 1){
        aiClass *ai = new aiClass(aiCard1, aiCard2);
        vector<int> passCards = {cards[5],cards[6],cards[7]};
        postFlopAction = ai->postFlop(passCards);
        delete ai;
        aiClassPost(postFlopAction);
    } else if (turn == 1  && !playerAllIn){
        aiClass *ai = new aiClass(aiCard1, aiCard2);
        vector<int> passCards = {cards[5],cards[6],cards[7],cards[9]};
        postFlopAction = ai->postFlop(passCards);
        delete ai;
        aiClassPost(postFlopAction);
    }
    else if (river == 1 && !playerAllIn){
            aiClass *ai = new aiClass(aiCard1, aiCard2);
            vector<int> passCards = {cards[5],cards[6],cards[7],cards[9],cards[11]};
            postFlopAction = ai->postFlop(passCards);
            delete ai;
            aiClassPost(postFlopAction);
        }

}

void holdEmGame::allIn() {


    ui->checkCallButton->hide();
    ui->foldButton->hide();

    // Show image of AI's first card
    QString text = getCardImagePath(aiCard1);
    QPixmap pCardPic1(text);
    ui->paiCard1->setPixmap(pCardPic1.scaled(ui->playerCard1->width(), ui->playerCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Show image of AI's second card
    text = getCardImagePath(aiCard2);
    QPixmap pCardPic2(text);
    ui->paiCard2->setPixmap(pCardPic2.scaled(ui->playerCard2->width(), ui->playerCard2->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    delay(1);

    if (pre == 1) {
        flopMethod();
        delay(2);
        turnMethod();
        delay(2);
        riverMethod();
        showDown();
    }
    if (flop == 1) {
        turnMethod();
        delay(2);
        riverMethod();
        showDown();
    }
    if (turn == 1) {
        riverMethod();
        showDown();
    }

}

void holdEmGame::showDown() {


    int playerCards[] = {pCard1, pCard2};
    int aiCards[] = {aiCard1, aiCard2};
    int commCards[] = {cards[5], cards[6], cards[7], cards[9], cards[11]};
    pokerRank *playerRank = new pokerRank(playerCards, commCards);
    pokerRank *aiRank = new pokerRank(aiCards, commCards);
    int player = playerRank->EvaluateHand();
    int ai = aiRank->EvaluateHand();


    // Show image of AI's first card
    QString text = getCardImagePath(aiCard1);
    QPixmap pCardPic1(text);
    ui->paiCard1->setPixmap(pCardPic1.scaled(ui->playerCard1->width(), ui->playerCard1->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Show image of AI's second card
    text = getCardImagePath(aiCard2);
    QPixmap pCardPic2(text);
    ui->paiCard2->setPixmap(pCardPic2.scaled(ui->playerCard2->width(), ui->playerCard2->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));



    if (player > ai) {
        playerWins();


    }
    else if (player < ai) {
        aiWins();


    }
    else if (player == ai) {
        if (player == 0) {
            for (int i = 0; i < 5; i++) {
                if (playerRank->bestHandVector[i] != aiRank->bestHandVector[i]) {
                    if (playerRank->bestHandVector[i] > aiRank->bestHandVector[i]) {
                        playerWins();
                        break;
                    }
                    else {
                        aiWins();
                        break;
                    }
                }
                else {
                    if (i == 4) {
                        chopPot();
                        break;
                    }
                    continue;
                }
            }

        }
        if (player == 1) {
            for (int i = 0; i < 5; i++) {
                if (playerRank->bestHandVector[i] != aiRank->bestHandVector[i]) {
                    if (playerRank->bestHandVector[i] > aiRank->bestHandVector[i]) {

                        playerWins();
                        break;
                    }
                    else {

                        aiWins();
                        break;
                    }
                }
                else {
                    if (i == 4) {

                        chopPot();
                        break;
                    }
                    continue;
                }
            }

        }
        if (player == 2) {
            for (int i = 0; i < 5; i = i + 2) {
                if (playerRank->bestHandVector[i] != aiRank->bestHandVector[i]) {
                    if (playerRank->bestHandVector[i] > aiRank->bestHandVector[i]) {
                        playerWins();

                        break;
                    }
                    else {

                        aiWins();
                        break;
                    }

                }
                else {
                    if (i == 4) {
                        chopPot();
                        break;
                    }
                    continue;
                }
            }


        }
        if (player == 3) {
            for (int i = 0; i < 5; i++) {
                if (playerRank->bestHandVector[i] != aiRank->bestHandVector[i]) {
                    if (playerRank->bestHandVector[i] > aiRank->bestHandVector[i]) {

                        playerWins();
                        break;
                    }
                    else {

                        aiWins();
                        break;
                    }
                }
                else {
                    if (i == 4) {

                        chopPot();
                        break;
                    }
                    continue;
                }
            }

        }
        if (player == 4) {
            if (playerRank->bestHandVector[0] > aiRank->bestHandVector[0]) {

            }
            else if (playerRank->bestHandVector[0] < aiRank->bestHandVector[0]) {

            }
            else if (playerRank->bestHandVector[0] == aiRank->bestHandVector[0]) {

            }
        }
        if (player == 5) {
            for (int i = 0; i < 5; i++) {
                if (playerRank->bestHandVector[i] != aiRank->bestHandVector[i]) {
                    if (playerRank->bestHandVector[i] > aiRank->bestHandVector[i]) {
                        playerWins();
                        break;
                    }
                    else {
                        aiWins();
                        break;
                    }
                }
                else {
                    if (i == 0) {
                        chopPot();
                        break;
                    }
                    continue;
                }
            }

        }
        if (player == 6) {
            if (playerRank->bestHandVector[0] > aiRank->bestHandVector[0]) {
                playerWins();
            }
            else if (playerRank->bestHandVector[0] < aiRank->bestHandVector[0]) {
                aiWins();
            }
            else if (playerRank->bestHandVector[0] == aiRank->bestHandVector[0]) {
                if (playerRank->bestHandVector[3] > aiRank->bestHandVector[3]) {
                    playerWins();
                }
                else if (playerRank->bestHandVector[3] < aiRank->bestHandVector[3]) {
                    aiWins();
                }
                else if (playerRank->bestHandVector[3] == aiRank->bestHandVector[3]) {
                    chopPot();
                }
            }
        }
        if (player == 7) {
            if (playerRank->bestHandVector[0] > aiRank->bestHandVector[0]) {
                playerWins();
            }
            else if (playerRank->bestHandVector[0] < aiRank->bestHandVector[0]) {
                aiWins();
            }
            else if (playerRank->bestHandVector[0] == aiRank->bestHandVector[0]) {
                if (playerRank->bestHandVector[1] > aiRank->bestHandVector[1]) {
                    playerWins();
                }
                else if (playerRank->bestHandVector[1] < aiRank->bestHandVector[1]) {
                    aiWins();
                }
                else if (playerRank->bestHandVector[1] == aiRank->bestHandVector[1]) {
                    chopPot();
                }
            }
        }
        if (player == 8) {
            if (playerRank->bestHandVector[0] > aiRank->bestHandVector[0]) {
                playerWins();
            }
            else if (playerRank->bestHandVector[0] < aiRank->bestHandVector[0]) {
                aiWins();
            }
            else if (playerRank->bestHandVector[0] == aiRank->bestHandVector[0]) {
                chopPot();
            }
        }

    }

    delay(3);


    delete playerRank;
    delete aiRank;

    if (ui->playerStackLabel->text().split("$")[1].toInt() == 0 || ui->aiStackLabel->text().split("$")[1].toInt() == 0) {
            gameEnds();
    } else {
            buttonChange();
            dealCards();
    }

}

void holdEmGame::gameEnds() {
    ui->paiCard1->hide();
    ui->paiCard2->hide();
    ui->playerCard1->hide();
    ui->playerCard2->hide();
    if (ui->playerStackLabel->text().split("$")[1].toInt() == 0) {
       // QMessageBox::information(this, "Winner", "You lost");
    } else {
        //QMessageBox::information(this, "Winner", "Congratulations, you won");
    }

    ui->pushButton->show();
}


void holdEmGame::hideCommCards() {
    ui->communityCard1->clear();
    ui->communityCard2->clear();
    ui->communityCard3->clear();
    ui->communityCard4->clear();
    ui->communityCard5->clear();
}

void holdEmGame::playerWins() {

     pStackSize =  ui->playerStackLabel->text().split("$")[1].toInt() + totalPot;
    ui->playerStackLabel->setText("$" + QString::number(pStackSize));


}

void holdEmGame::aiWins() {
    aiStackSize = ui->aiStackLabel->text().split("$")[1].toInt() + totalPot;;
    ui->aiStackLabel->setText("$" + QString::number(aiStackSize));

}

void holdEmGame::chopPot() {

    int half = totalPot / 2;
    pStackSize = ui->playerStackLabel->text().split("$")[1].toInt() + half;
    aiStackSize = ui->aiStackLabel->text().split("$")[1].toInt() + half;
    ui->aiStackLabel->setText("$" + QString::number(aiStackSize));
    ui->playerStackLabel->setText("$" + QString::number(pStackSize));


}


void holdEmGame::delay(int n) {
    QTime dieTime = QTime::currentTime().addSecs(n);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void holdEmGame::on_pushButton_clicked() {

    ui->pushButton->hide();
    ui->playerStackLabel->setText("$" + QString::number(startingStack));
    ui->aiStackLabel->setText("$" + QString::number(startingStack));
    delay(1);
    // Call methods for game to commence
    buttonGen();
    dealCards();
}
