#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class DbConnectException : public std::exception
{
public:
    const char* what() const noexcept
    {
        return "Cannot connect to database!";
    }
};

class Database
{
public:
    Database();
    ~Database();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
