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

class EndPointWebSocket;
class MsgHandler;


class WebSocketSession : public QObject
{
        Q_OBJECT
public:
        explicit WebSocketSession(QWebSocket &ws,
                         EndPointWebSocket const &endpoint);

        /**
         * @brief Close socket and schedule it for defferred deletion.
         */
        ~WebSocketSession();

        WebSocketSession(WebSocketSession const &) = delete;
        WebSocketSession& operator=(WebSocketSession const &) = delete;

        /**
         * @brief Forwards the call to signal bin_msg_tx.
         */
        void bin_msg_tx(QByteArray raw_msg);

public slots:
        void on_text_msg_rx(QString msg);
        void on_bin_msg_rx(QByteArray raw_msg);

signals:
        void signal_bin_msg_tx(QWebSocket *ws, QByteArray raw_msg);

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
        QWebSocket &ws;

        EndPointWebSocket const& endpoint;

        QString         user;
        bool            logged_in;

private:

        /*
         * Connections are handled by worker threads from thread pool.
         */
        static QThreadPool      thread_pool;
};

#endif // WEBSOCKETSESSION_H
