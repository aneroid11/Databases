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

    void on_artistAccDetails_commentsButton_clicked();

    void on_comments_backButton_clicked();

    void on_comments_deleteButton_clicked();

    void on_artistAccDetails_reportsButton_clicked();

    void on_artistAccDetails_playlistsButton_clicked();

    void on_playlists_backButton_clicked();

    void on_playlists_deleteButton_clicked();

    void on_artistAcc_artistsButton_clicked();

    void on_myTracks_uploadTrackButton_clicked();

    void on_myTrackEdit_backButton_clicked();

    void on_myTrackEdit_commentsButton_clicked();

    void on_myTrackEdit_addNewTagButton_clicked();

    void on_myTrackEdit_deleteTagButton_clicked();

    void on_myTrackEdit_saveButton_clicked();

    void on_artistAcc_allPlaylistsButton_clicked();

    void on_playlists_detailsButton_clicked();

    void on_playlistDetails_exit_clicked();

    void on_playlistDetails_listen_clicked();

    void on_playlistDetails_addTag_clicked();

    void on_playlistDetails_deleteTag_clicked();

    void on_playlistDetails_deleteTrack_clicked();

    void on_playlistDetails_save_clicked();

    void on_allTracks_addToPlaylist_clicked();

    void on_myTracks_addToPlaylistButton_clicked();

    void on_playlists_createButton_clicked();

    void on_playlistCreation_cancel_clicked();

    void on_playlistCreation_create_clicked();

    void on_playlistCreation_isAlbum_stateChanged(int arg1);

    void on_allTracks_listenButton_clicked();

    void on_allTracks_likeButton_clicked();

    void on_allTracks_commentButton_clicked();

    void on_allTracks_reportButton_clicked();

    void on_artists_reportButton_clicked();

    void on_allTracks_filterButton_clicked();

    void on_artistAcc_cardDetailsButton_clicked();

    void on_cardDetails_cancel_clicked();

    void on_cardDetails_save_clicked();

private:
    int getCurrentItemId(QListWidget* list);

    bool yesNoQuestion(QString msg);
    bool areYouSure();
    void showMsg(QString msg);

    void artistPageInit();
    void adminPageInit();
    void reportsPageInit(const int artistId = -1);
    void artistsPageInit();
    void allTracksPageInit();
    void artistAccDetailsPageInit(const int artistId);
    void myTracksPageInit(const int artistId);
    void commentsPageInit(const int artistId);
    void playlistsPageInit(const int artistId = -1);
    void playlistDetailsPageInit(const int playlistId);
    void myTrackEditPageInit(const int trackId);
    void cardDetailsInit();

    void deleteCurrAcc();

    void fillReportsList(QListWidget* listWidget, const QList<Report>& reports);
    void fillReportsList();
    void fillTracksList(QListWidget* listWidget, const QList<TrackInfo>& tracks);
    void fillTracksList();
    void fillArtistsList();
    void fillCommentsList(QListWidget* listWidget, const QList<DataRow>& comments);
    void fillPlaylistsList(QListWidget* listWidget, const QList<DataRow>& playlists);

    void fillTags(QListWidget* listWidget, const int trackId);
    void fillTagsFor(QListWidget *listWidget, const QString type, const int id);
    void fillAllTags(QListWidget *listWidget);

    void addCurrTrackToPlaylist(QListWidget* tracksListWidget);

    void reportArtistOrTrack(const QString reportType, QListWidget* itemsList);

    int extractIdFromBeginning(QString str);

    Ui::MainWindow *ui;

    Database* db;
};
#endif // MAINWINDOW_H
