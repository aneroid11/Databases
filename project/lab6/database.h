#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QList>

struct Report
{
    int id;
    QString title;
    QString contents;
    int authorId;
    QString reportType;
    int objectId;
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

private:
    int numAdmins();

    void prepareExec(QSqlQuery& q, QString queryStr);

    QSqlDatabase db;

    QString currUserRole = QString();
    int currUserId = -1;
};

#endif // DATABASE_H
