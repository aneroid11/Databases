#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_starting_signInButton_clicked();

    void on_starting_signUpButton_clicked();

    void on_signUp_signUpButton_clicked();

    void on_signIn_signInButton_clicked();

    void on_stackedWidget_currentChanged(int index);

    void on_artistAcc_signOffButton_clicked();

    void on_artistAcc_deleteAccountButton_clicked();

    void on_artistAcc_accountButton_clicked();

    void on_artistAccDetails_cancelButton_clicked();

    void on_artistAccDetails_saveChangesButton_clicked();

    void on_artistAcc_allTracksButton_clicked();

    void on_allTracks_exitButton_clicked();

    void on_artistAcc_myTracksButton_clicked();

    void on_myTracks_backButton_clicked();

    void on_myTracks_listenButton_clicked();

    void on_myTracks_deleteButton_clicked();

    void on_myTracks_detailsButton_clicked();

private:
    bool areYouSure();
    void showMsg(QString msg);

    void artistPageInit();

    Ui::MainWindow *ui;

    Database* db;
};
#endif // MAINWINDOW_H
