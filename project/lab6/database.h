#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>

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

private:
    void prepareExec(QSqlQuery& q, QString queryStr);

    QSqlDatabase db;

    QString currUserRole = QString();
    int currUserId = -1;
};

#endif // DATABASE_H
