#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QMessageBox>
#include "holdemgame.h"
#include "pokerrank.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap background(":/cards/cards/homeBackground.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    if (ui->lineEdit->text() != "") {
    holdEm = new holdEmGame(this, name);
    holdEm->show();



    }   else {
        QMessageBox::information(this, "Login", "Please enter a name");
}
}
