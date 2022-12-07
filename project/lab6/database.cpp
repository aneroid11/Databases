#include "database.h"

#include <exception>

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
        throw DbConnectException();
    }
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close();
    }
}
