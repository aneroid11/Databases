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
        throw QString("No such user!");
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
