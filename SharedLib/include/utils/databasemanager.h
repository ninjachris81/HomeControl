#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include "appconfiguration.h"

class DatabaseManager
{

public:
    explicit DatabaseManager();

    static DatabaseManager* m_instance;

    static DatabaseManager* instance() {
        if (m_instance==nullptr) {
            m_instance = new DatabaseManager();
        }

        return m_instance;
    }

    static QString DATABASE_NAME;
    static QString DATABASE_USERNAME;
    static QString DATABASE_PASSWORD;

    void init(AppConfiguration *appConfig);

    QSqlDatabase db();

    bool supportsQuerySize();

protected:
    QSqlDatabase m_db;

public slots:
};

#endif // DATABASEMANAGER_H
