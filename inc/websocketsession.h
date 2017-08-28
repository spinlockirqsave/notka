#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QByteArray>
#include <QMutex>
#include <QRunnable>
#include <QDataStream>
#include <QIODevice>
#include <QScopedPointer>
#include <QThreadPool>

#include "../inc/msghandler.h"


class WebSocketSession : public QObject
{
        Q_OBJECT
public:
        WebSocketSession(QWebSocket *ws) :
                mutex(),
                qrunnables_scheduled(0),
                ws(ws),
                user(),
                logged_in(false)
        {
                connect(ws, &QWebSocket::textMessageReceived,
                        this, &WebSocketSession::on_text_msg_rx);
                connect(ws, &QWebSocket::binaryMessageReceived,
                        this, &WebSocketSession::on_bin_msg_rx);
        }

        /**
         * @brief Close socket and schedule it for defferred deletion.
         */
        ~WebSocketSession();

        WebSocketSession(WebSocketSession const &) = delete;
        WebSocketSession& operator=(WebSocketSession const &) = delete;

public slots:
        void on_text_msg_rx(QString msg);
        void on_bin_msg_rx(QByteArray raw_msg);

public:
        /**
         * @brief Mutex serialising access to this session.
         * Accessed by MsgHandler in QRunnable's run().
         */
        QMutex          mutex;
        int             qrunnables_scheduled;

/**
 * TODO this should be private and session made QRunnable...
 * instead of MsgHandler being QRunnable and processing session
 * which requires session's user data to be public, or MsgHandler
 * being session's friend, what's worse...
 */
        QWebSocket      *ws;
        QString         user;
        bool            logged_in;

private:

        /*
         * Connections are handled by worker threads from thread pool.
         */
        static QThreadPool      thread_pool;
};

#endif // WEBSOCKETSESSION_H
