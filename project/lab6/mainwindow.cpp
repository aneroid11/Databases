#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QDebug>
#include <QMessageBox>

const int STARTING_PAGE_ID = 0;
const int SIGNIN_ID = 1;
const int SIGNUP_ID = 2;
const int ARTIST_ACC_PAGE_ID = 3;
const int MODERATOR_ACC_PAGE_ID = 4;
const int ADMIN_ACC_PAGE_ID = 5;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_signInButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(SIGNIN_ID);
}

void MainWindow::on_signUpPageButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);
}

void MainWindow::on_signUpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(SIGNUP_ID);
}

void MainWindow::on_submitButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
}

void MainWindow::artistPageInit()
{
    ui->label_3->setText(QString("artist") + QString::number(rand()));
}


void MainWindow::on_stackedWidget_currentChanged(int index)
{
    qDebug() << "on_stackedWidget_currentChanged()\n";

    switch (index)
    {
    case ARTIST_ACC_PAGE_ID:
        artistPageInit();
        break;
    }
}
