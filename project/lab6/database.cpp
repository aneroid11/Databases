#include "database.h"
#include "sha256.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include <QDebug>

Database::Database()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("task3");
    db.setUserName("root");
    db.setPassword("msql_pss_rt");
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
    // проверить, есть ли юзер с таким емейлом
    // если есть, проверить его пароль
    // если совпадает, то выставить @curr_session_user_id = user id.

    QString queryStr = QString("select 1 from Users where email = '%1' and passwordHash = %2")
            .arg(email)
            .arg(sha256hash(password));

    qDebug() << queryStr;

    QSqlQuery q;
    prepareExec(q, queryStr);

    if (!q.next())
    {
        throw QString("No such user!");
    }
    const int userId = q.value(0).toInt();

    queryStr = QString("set @curr_session_user_id = %1;").arg(userId);
    prepareExec(q, queryStr);
}

void Database::prepareExec(QSqlQuery q, QString queryStr)
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
