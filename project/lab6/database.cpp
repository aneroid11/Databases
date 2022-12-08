#include "database.h"
#include "sha256.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlResult>
#include <QDebug>

Database::Database()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("task3");
    db.setUserName("root");
    db.setPassword("msql_pss_rt");
    db.setPort(3306);
    bool ok = db.open();

    if (!ok)
    {
        throw QString("cannot connect to db!");
    }
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close();
    }
}

void Database::prepareExec(QSqlQuery& q, QString queryStr)
{
    if (!q.prepare(queryStr))
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }
    if (!q.exec())
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }
}

void Database::prepareExecWithBinding(QSqlQuery &q, QString qStr, const QList<QVariant> &params)
{
    if (!q.prepare(qStr))
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }

    for (int i = 0; i < params.size(); i++)
    {
        q.bindValue(i, params[i]);
    }

    if (!q.exec())
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }
}

Report Database::extractReportFromQuery(const QSqlQuery &q)
{
    Report newReport;
    newReport.id = q.value(0).toInt();
    newReport.title = q.value(1).toString();
    qDebug() << newReport.title;
    newReport.contents = q.value(2).toString();
    newReport.authorId = q.value(3).toInt();
    newReport.reportType = q.value(4).toString();
    newReport.objectId = q.value(5).toInt();
    return newReport;
}

Artist Database::extractArtistFromQuery(const QSqlQuery &q)
{
    Artist a;
    a.id = q.value(0).toInt();
    a.email = q.value(1).toString();
    a.passwordHash = q.value(2).toString();
    a.nickname = q.value(3).toString();
    a.dateOfBirth = q.value(4).toString();
    a.gender = q.value(5).toString();
    a.premiumSubscriptionId = q.value(6).toInt();
    a.cardDetailsId = q.value(7).toInt();
    return a;
}

void Database::signUpArtist(QString email,
                            QString passwordHash,
                            QString nickname,
                            QString dateOfBirth,
                            QString gender)
{
    QSqlQuery q;
    //prepareExec(q, queryStr);
    prepareExecWithBinding(q,
                           "call RegisterArtist(:email, :pwdHash, :nickname, :dateOfBirth, :gender);",
                           { email, passwordHash, nickname, dateOfBirth, gender });
}

void Database::signInUser(QString email, QString password)
{
    /*QString queryStr = QString("select id, role from Users where email = '%1' and password_hash = '%2';")
            .arg(email)
            .arg(sha256hash(password));*/

    //qDebug() << queryStr;

    QSqlQuery q;
    //prepareExec(q, queryStr);
    prepareExecWithBinding(q,
                           "select id, role from Users where email = :email and password_hash = :pwdHash;",
                           { email, sha256hash(password) });

    if (!q.next())
    {
        throw QString("No user with such email and/or password!");
    }
    else
    {
        qDebug() << q.value(0).toInt() << "\n";
    }
    const int userId = q.value(0).toInt();
    const QString userRole = q.value(1).toString();
    currUserId = userId;
    currUserRole = userRole;

    prepareExec(q, QString("set @curr_session_user_id = %1;").arg(userId));
}

void Database::signOffCurrUser()
{
    currUserId = -1;
    currUserRole = QString();

    QString qStr = "set @curr_session_user_id = NULL;";
    QSqlQuery q;
    prepareExec(q, qStr);

    qDebug() << currUserId << " is currUserId";
    qDebug() << currUserRole << " is currUserRole";
}

QString Database::getCurrArtistNickname()
{
    if (currUserId < 0 || currUserRole != "artist")
    {
        return QString();
    }

    QString qStr = QString("select nickname from Artists where id = %1").arg(currUserId);
    QSqlQuery q;
    prepareExec(q, qStr);
    q.next();
    return q.value(0).toString();
}

QString Database::getCurrUserEmail()
{
    if (currUserId < 0) { return QString(); }

    QString qStr = QString("select email from Users where id = %1").arg(currUserId);
    QSqlQuery q;
    prepareExec(q, qStr);
    q.next();
    return q.value(0).toString();
}

int Database::numAdmins()
{
    QString qStr = QString("select count(id) from Users where role = 'admin';");
    QSqlQuery q;
    prepareExec(q, qStr);
    q.next();
    return q.value(0).toInt();
}

void Database::deleteAccount(int id)
{
    if (currUserRole == "artist")
    {
        QString qStr = QString("call DeleteArtistAccount(%1);").arg(id);
        QSqlQuery q;
        prepareExec(q, qStr);
    }
    else if (currUserRole == "admin")
    {
        if (numAdmins() < 2)
        {
            throw QString("Cannot have no admins!");
        }

        QString qStr = QString("delete from Users where id = %1;").arg(id);
        QSqlQuery q;
        prepareExec(q, qStr);
    }

    signOffCurrUser();
}

void Database::createAdmin(QString email, QString password)
{
    /*QString qStr = QString("insert into Users (email, password_hash, role) values ('%1', '%2', 'admin');")
            .arg(email)
            .arg(sha256hash(password));*/
    QString qStr = "insert into Users (email, password_hash, role) values (:email, :pwdHash, 'admin');";
    QSqlQuery q;
    prepareExecWithBinding(q, qStr, { email, sha256hash(password) });
}

QList<Report> Database::getAllReports()
{
    QString s = QString("select * from Reports;");
    QSqlQuery q;
    prepareExec(q, s);
    QList<Report> ret;

    while (q.next())
    {
        ret.push_back(extractReportFromQuery(q));
    }

    return ret;
}

Report Database::getReport(int id)
{
    QString s = QString("select * from Reports where id = %1;").arg(id);
    QSqlQuery q;
    prepareExec(q, s);
    if (!q.next())
    {
        throw QString("No report with such id!");
    }

    return extractReportFromQuery(q);
}

QList<Artist> Database::getAllArtists()
{
    QString s = QString("select * from ArtistsInfo;");
    QSqlQuery q;
    prepareExec(q, s);

    QList<Artist> ret;

    while (q.next())
    {
        ret.push_back(extractArtistFromQuery(q));
    }

    return ret;
}

Artist Database::getArtistInfo(const int id)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "select * from ArtistsInfo where id = :id", { id });

    if (!q.next()) { throw QString("No artist with id = %1!").arg(id); }

    return extractArtistFromQuery(q);
}

QList<TrackInfo> Database::getAllTracksInfo()
{
    QSqlQuery q;
    prepareExec(q, "select * from TracksInfo;");
    QList<TrackInfo> ret;

    while (q.next())
    {
        TrackInfo info;
        info.id = q.value(0).toInt();
        info.timestamp = q.value(1).toString();
        info.title = q.value(2).toString();
        info.lengthSeconds = q.value(3).toInt();
        info.artistNickname = q.value(4).toString();
        ret.push_back(info);
    }

    return ret;
}

void Database::deleteReport(int id)
{
    QString s = QString("delete from Reports where id = %1;").arg(id);
    QSqlQuery q;
    prepareExec(q, s);
}

void Database::deleteTrack(int id)
{
    QString s = QString("delete from Tracks where id = %1;").arg(id);
    QSqlQuery q;
    prepareExec(q, s);
}
