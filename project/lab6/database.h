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

    QList<Report> getAllReports();
    Report getReport(int id);

    QList<Artist> getAllArtists();
    Artist getArtistInfo(const int id);

    QList<TrackInfo> getAllTracksInfo();
    QList<TrackInfo> getTracksInfo(const int artistId);

    void deleteReport(int id);

    void deleteTrack(int id);

    int getCurrUserId() const { return currUserId; }

    QList<DataRow> getCommentsBy(const int artistId);

    void deleteRecordFromTable(QString tableName, const int id);

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
