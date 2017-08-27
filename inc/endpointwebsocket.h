#ifndef ENDPOINTWEBSOCKET_H
#define ENDPOINTWEBSOCKET_H


#include <QObject>
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QThread>
#include <QMutex>
#include <QThreadPool>

#include "../inc/notkaendpoint.h"
#include "../inc/msghandler.h"


/**
 * @brief       Web sockets endpoint.
 *              Runs web server over Web Socket protocol
 *              and handles communication with web socket clients.
 */
class EndPointWebSocket : public QObject, public NotkaEndPoint
{
        Q_OBJECT
public:
        /**
         * @param       mode        Web socket security mode (ws/wss).
         */
        explicit EndPointWebSocket(QWebSocketServer::SslMode mode,
                                   QHostAddress address,
                                   quint16 port,
                                   QObject *parent = Q_NULLPTR);
        ~EndPointWebSocket();

        EndPointWebSocket(const EndPointWebSocket &) = delete;
        EndPointWebSocket& operator=(const EndPointWebSocket &) = delete;

        int open() override;
        void close() override;

signals:
        void closed();

private slots:
        /**
         * @brief Gracefully close connections.
         */
        void ws_clients_disconnect();

        void on_new_connection();
        void on_text_msg_rx(QString msg);

        /**
         * @brief Callback from the Web Socket.
         * @param msg   Incoming binary message.
         */
        void on_bin_msg_rx(QByteArray raw_msg);

        void on_sock_disconnect();

private:
        /**
         * Scoped pointer asserts the Web Socket server will not outlive
         * the object responsible for it, i.e. the EndPointWebSocket which created it.
         */
        QScopedPointer<QWebSocketServer>        ws_server;

        QHostAddress                            address;
        quint16                                 port;

        /**
         * QWebSocketServer owns the pointers to the sockets.
         */
        QMutex                                  ws_clients_mutex;
        QList<QWebSocket*>                      ws_clients;

        /*
         * Connections are handled by worker threads from thread pool.
         */
        QThreadPool                             thread_pool;
};

#endif // ENDPOINTWEBSOCKET_H
