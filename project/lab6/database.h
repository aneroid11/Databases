#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QList>
#include <QMap>

struct Report
{
    int id;
    QString title;
    QString contents;
    int authorId;
    QString reportType;
    int objectId;
};

struct Artist
{
    int id;
    QString email;
    QString passwordHash;
    QString nickname;
    QString dateOfBirth;
    QString gender;
    int premiumSubscriptionId;
    int cardDetailsId;
};

struct TrackInfo
{
    int id;
    QString timestamp;
    QString title;
    int lengthSeconds;
    QString artistNickname;
};

/*struct Comment
{

};*/

struct DataRow
{
    //QList<QVariant> data;
    // columns names and values
    QMap<QString, QVariant> data;
};

class Database
{
public:
    Database();
    ~Database();

    void signUpArtist(QString email,
                      QString passwordHash,
                      QString nickname,
                      QString dateOfBirth,
                      QString gender);

    void signInUser(QString email,
                    QString password);

    void signOffCurrUser();

    QString getCurrUserRole() const { return currUserRole; }

    QString getCurrArtistNickname();
    QString getCurrUserEmail();

    void deleteAccount(int id);
    void deleteCurrAccount() { deleteAccount(currUserId); }

    void createAdmin(QString email, QString password);

    QList<Report> getReportsByArtist(const int artistId);
    QList<Report> getAllReports();
    Report getReport(int id);

    QList<Artist> getAllArtists();
    Artist getArtistInfo(const int id);

    DataRow getFromTableById(const QString table, const int id);

    void updateDataIn(const QString table, const int id, const DataRow& data);
    void insertDataInto(const QString table, const DataRow& data);
    void attachCardDetails(const int artistId, const DataRow& card);

    void disablePremium(int premiumSubscriptionId);
    void enablePremium(int premiumSubscriptionId);
    void createPremium(int artistId);

    void createPaymentForMonth(const int premiumId, const QString status, const QString transactionId);

    QList<TrackInfo> getAllTracksInfo();
    QList<TrackInfo> getTracksInfo(const int artistId);
    TrackInfo getTrackInfo(const int id);
    QList<TrackInfo> getTracksFromPlaylistInfo(int playlistId);

    QList<TrackInfo> searchTracksByTitle(QString title);
    QList<TrackInfo> searchTracksByTitleAndTags(QString title, QStringList tags);

    int numLikesOnTrack(const int trackId);

    QList<DataRow> getCommentsOnTrack(const int trackId);

    void deleteReport(int id);

    void createTrack(int artistId, QString title, int length);
    void deleteTrack(int id);

    int getCurrUserId() const { return currUserId; }

    QList<DataRow> getCommentsBy(const int artistId);

    void deleteRecordFromTable(QString tableName, const int id);

    QList<DataRow> getAllPlaylists();
    QList<DataRow> getPlaylistsBy(const int artistId);
    DataRow getPlaylist(const int playlistId);
    int idPlaylistOwner(const int playlistId);

    void deletePlaylist(const int id);
    void addTrackToPlaylist(const int playlistId, const int currTrackId);
    void deleteTrackFromPlaylist(const int trackId, const int playlistId);

    void updatePlaylistTitle(const int playlistId, const QString newTitle);
    void updateAlbumReleaseDate(const int playlistId, const QString newDate);

    void updateArtist(int id, QString email, QString nickname, QString password, QString dateOfBirth, QString gender);

    void updateTrackTitle(int trackId, QString title);

    QStringList getTrackTags(const int trackId);
    QStringList getTagsFor(QString table, int id);
    QStringList getAllTags();

    void attachTagToTrack(const int trackId, const QString newTag);
    void deattachTagFromTrack(const int trackId, const QString tag);

    void attachTagToPlaylist(const int playlistId, const QString newTag);
    void deattachTagFromPlaylist(const int playlistId, const QString tag);

    void createPlaylist(const int artistId, const QString title);
    void createAlbum(const int artistId, const QString title, const QDate releaseDate);

    void addLike(const int artistId, const int trackId);
    void addComment(const int artistId, const int trackId, const QString contents);

    void reportTrack(int artistId, int trackId, QString title, QString contents);
    void reportArtist(int artistId, int reportedArtistId, QString title, QString contents);

private:
    int numAdmins();

    void prepareExec(QSqlQuery& q, QString queryStr);
    void prepareExecWithBinding(QSqlQuery& q, QString qStr, const QList<QVariant> &params);

    Report extractReportFromQuery(const QSqlQuery& query);
    Artist extractArtistFromQuery(const QSqlQuery& q);
    TrackInfo extractTrackInfoFromQuery(const QSqlQuery& q);
    DataRow extractDataRowFromQuery(const QSqlQuery& q);

    QSqlDatabase db;

    QString currUserRole = QString();
    int currUserId = -1;
};

#endif // DATABASE_H
