#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QDebug>
#include <QMessageBox>

enum {
    STARTING_PAGE_ID,
    SIGNIN_PAGE_ID,
    SIGNUP_PAGE_ID,
    ARTIST_ACC_PAGE_ID,
    MODERATOR_ACC_PAGE_ID,
    ADMIN_ACC_PAGE_ID,
    ARTIST_ACC_DETAILS_PAGE_ID,
    ALL_TRACKS_PAGE_ID
};

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

bool MainWindow::areYouSure()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure?", "Do you really want to do that?",
                                  QMessageBox::Yes|QMessageBox::No);
    return reply == QMessageBox::Yes;
}

void MainWindow::showMsg(QString msg)
{

    QMessageBox msgBox;
    msgBox.setWindowTitle("Message");
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::on_starting_signInButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(SIGNIN_PAGE_ID);
}

void MainWindow::on_starting_signUpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(SIGNUP_PAGE_ID);
}

void MainWindow::on_signUp_signUpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);
}

void MainWindow::on_signIn_signInButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
}

void MainWindow::artistPageInit()
{
    ui->artistAcc_nicknameLabel->setText(QString("artist") + QString::number(rand()));
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

void MainWindow::on_artistAcc_signOffButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);
}

void MainWindow::on_artistAcc_deleteAccountButton_clicked()
{
    if (areYouSure()) {
        ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);

        showMsg("Your account was deleted");
    }
}

void MainWindow::on_artistAcc_accountButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
}

void MainWindow::on_artistAccDetails_cancelButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
}

void MainWindow::on_artistAccDetails_saveChangesButton_clicked()
{
    if (areYouSure())
    {
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
    }
}

void MainWindow::on_artistAcc_allTracksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ALL_TRACKS_PAGE_ID);
}

void MainWindow::on_allTracks_exitButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
}
