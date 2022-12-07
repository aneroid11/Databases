#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
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
    qDebug() << "gender = " << gender;

    QString queryStr = QString("call RegisterArtist("
                               "'%1', '%2', '%3', '%4', '%5');")
            .arg(email)
            .arg(passwordHash)
            .arg(nickname)
            .arg(dateOfBirth)
            .arg(gender);

    QSqlQuery q;
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
