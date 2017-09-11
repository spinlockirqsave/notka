#include "../inc/globals.h"

#include <QDebug>


QSqlDatabase& Db::instance()
{
        static QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        return db;
}

bool Db::init_database()
{
        QMutexLocker lock(&Database::mutex);

        QSqlDatabase& db = instance();

        db.setHostName("localhost");
        db.setDatabaseName("notka");
        db.setUserName("notka");
        db.setPassword("password");

        return db.open();
}

void Db::close_database()
{
        QSqlDatabase& db = instance();
        db.close();
}

bool Db::authenticate_user(QString login, QString password)
{
        QSqlDatabase &db = Db::instance();

        if (!db.isOpen() || db.isOpenError())
                throw std::runtime_error("Database is not opened");

        QMutexLocker lock(&Db::mutex);

        QSqlQuery query(db);
        query.exec("SELECT password FROM users WHERE user = '" + login + "'");

        if (query.lastError().isValid()) {
                throw std::runtime_error("Database query error"
                                         + query.lastError().text().toStdString());
        }

        while (query.next())
        {
                QString pass = query.value(0).toString();
                if (pass == password)
                        return true;
        }

        return false;
}

bool Db::save_notka(QString user, QByteArray notka)
{
        QMutexLocker lock(&Db::mutex);

        QSqlDatabase &db = Db::instance();

        db.transaction();

        QSqlQuery query(db);
        query.prepare( "INSERT INTO notkas (user, notka) VALUES (:user, :notka) "
                       "ON DUPLICATE KEY UPDATE notka = VALUES(notka)" );
        query.bindValue(0, user);
        query.bindValue(1, notka);
        query.exec();

        if (query.lastError().isValid()) {
                qDebug() << query.lastError().text();
                QSqlDatabase::database().rollback();
                return false;
        }

        db.commit();

        return true;
}
