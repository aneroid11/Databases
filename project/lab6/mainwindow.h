#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

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

    void on_myTracks_backButton_clicked();

    void on_myTracks_listenButton_clicked();

    void on_myTracks_deleteButton_clicked();

    void on_myTracks_detailsButton_clicked();

    void on_adminAcc_signOffButton_clicked();

    void on_adminAcc_deleteMyAccButton_clicked();

    void on_adminAcc_createAdminButton_clicked();

    void on_createAdmin_cancelButton_clicked();

    void on_createAdmin_createButton_clicked();

    void on_adminAcc_reportsButton_clicked();

    void on_reports_backButton_clicked();

    void on_reports_detailsButton_clicked();

    void on_reports_deleteButton_clicked();

    void on_adminAcc_artistsButton_clicked();

    void on_adminAcc_tracksButton_clicked();

    void on_allTracks_deleteButton_clicked();

    void on_artists_backButton_clicked();

    void on_artists_detailsButton_clicked();

    void on_artists_deleteButton_clicked();

    void on_artistAccDetails_tracksButton_clicked();

private:
    int getCurrentItemId(QListWidget* list);

    bool areYouSure();
    void showMsg(QString msg);

    void artistPageInit();
    void adminPageInit();
    void reportsPageInit();
    void artistsPageInit();
    void allTracksPageInit();
    void artistAccDetailsPageInit(const int artistId);
    void myTracksPageInit(const int artistId);

    void deleteCurrAcc();

    void fillReportsList();
    void fillTracksList(QListWidget* listWidget, const QList<TrackInfo>& tracks);
    void fillTracksList();
    void fillArtistsList();

    int extractIdFromBeginning(QString str);

    Ui::MainWindow *ui;

    Database* db;
};
#endif // MAINWINDOW_H
