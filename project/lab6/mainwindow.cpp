#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sha256.h"

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
    ALL_TRACKS_PAGE_ID,
    MY_TRACKS_PAGE_ID,
    MY_TRACK_EDIT_PAGE_ID,
    MY_PLAYLISTS_PAGE_ID
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new Database();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
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
    const QString email = ui->signUp_emailLineEdit->text();
    if (email.isEmpty())
    {
        showMsg("Email line is empty!");
        return;
    }
    const QString pwd = ui->signUp_passwordLineEdit->text();
    if (pwd.isEmpty())
    {
        showMsg("Password line is empty!");
        return;
    }
    const QString nickname = ui->signUp_nicknameLineEdit->text();
    if (nickname.isEmpty())
    {
        showMsg("Nickname line is empty!");
        return;
    }

    try
    {
        db->signUpArtist(email, sha256hash(pwd), nickname,
                         ui->signUp_birthDateEdit->date().toString("yyyy-MM-dd"),
                         ui->signUp_genderComboBox->currentText());
    }
    catch (QString msg)
    {
        showMsg(msg);
        return;
    }

    /*try
    {

    }
    catch (const std::exception& exc)
    {
        showMsg(QString("Error: ") + exc.what());
        return;
    }*/

    showMsg("Signed up successfully!");

    ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);
}

void MainWindow::on_signIn_signInButton_clicked()
{
    const QString email = ui->signIn_emailLineEdit->text();
    if (email.isEmpty())
    {
        showMsg("Email line is empty!");
        return;
    }
    const QString pwd = ui->signIn_passwordLineEdit->text();
    if (pwd.isEmpty())
    {
        showMsg("Password line is empty!");
        return;
    }

    try
    {
        db->signInUser(email, pwd);
    }
    catch (QString msg)
    {
        showMsg(msg);
        return;
    }

    showMsg("Signed in successfully!");

    const QString role = db->getCurrUserRole();
    int newIndex;

    if (role == "artist") { newIndex = ARTIST_ACC_PAGE_ID; }
    else if (role == "admin") { newIndex = ADMIN_ACC_PAGE_ID; }
    else
    {
        showMsg(QString("What is this role: %1?").arg(role));
        return;
    }

    ui->stackedWidget->setCurrentIndex(newIndex);
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

void MainWindow::on_artistAcc_myTracksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(MY_TRACKS_PAGE_ID);
}

void MainWindow::on_myTracks_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
}

void MainWindow::on_myTracks_listenButton_clicked()
{
    showMsg("...wonderful music plays...");
}

void MainWindow::on_myTracks_deleteButton_clicked()
{
    if (areYouSure())
    {
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
        showMsg("The track was deleted");
    }
}

void MainWindow::on_myTracks_detailsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(MY_TRACK_EDIT_PAGE_ID);
}
