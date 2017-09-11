#ifndef GLOBALS_H
#define GLOBALS_H


#include <QMutexLocker>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


namespace Database {
        extern QMutex mutex;

        QSqlDatabase& instance();
        bool init_database();
        void close_database();
        int authenticate_user(QString login, QString password);
        bool save_notka(QString user, QByteArray notka);
}

namespace Db = Database;

#endif // GLOBALS_H
