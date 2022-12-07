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
        //throw DbConnectException();
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

void Database::signUpArtist(QString email,
                            QString passwordHash,
                            QString nickname,
                            QString dateOfBirth,
                            QString gender)
{
    QString queryStr = QString("call RegisterArtist("
                               "'%1', '%2', '%3', '%4', '%5');")
            .arg(email)
            .arg(passwordHash)
            .arg(nickname)
            .arg(dateOfBirth)
            .arg(gender);

    QSqlQuery q;
    prepareExec(q, queryStr);
}

void Database::signInUser(QString email, QString password)
{
    QString queryStr = QString("select id, role from Users where email = '%1' and password_hash = '%2';")
            .arg(email)
            .arg(sha256hash(password));

    qDebug() << queryStr;

    QSqlQuery q;
    prepareExec(q, queryStr);

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

    queryStr = QString("set @curr_session_user_id = %1;").arg(userId);
    prepareExec(q, queryStr);
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
    QString qStr = QString("insert into Users (email, password_hash, role) values ('%1', '%2', 'admin');")
            .arg(email)
            .arg(sha256hash(password));
    QSqlQuery q;
    prepareExec(q, qStr);
}

/*  int id;
    QString title;
    QString contents;
    int authorId;
    QString reportType;
    int objectId;
 * */

QList<Report> Database::getAllReports()
{
    QString s = QString("select * from Reports;");
    QSqlQuery q;
    prepareExec(q, s);
    QList<Report> ret;

    while (q.next())
    {
        Report newReport;
        newReport.id = q.value(0).toInt();
        newReport.title = q.value(1).toString();
        qDebug() << newReport.title;
        newReport.contents = q.value(2).toString();
        newReport.authorId = q.value(3).toInt();
        newReport.reportType = q.value(4).toString();
        newReport.objectId = q.value(5).toInt();
        ret.push_back(newReport);
    }

    return ret;
}
