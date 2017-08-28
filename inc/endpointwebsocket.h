#ifndef ENDPOINTWEBSOCKET_H
#define ENDPOINTWEBSOCKET_H


#include <QObject>
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QThread>
#include <QMutex>
#include <QThreadPool>
#include <QMap>

#include "../inc/notkaendpoint.h"
#include "../inc/msghandler.h"
#include "../inc/websocketsession.h"


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
        void ws_sessions_disconnect();

        void on_new_connection();

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
         * QWebSocketServer stores the pointers to the sessions.
         */
        QMap<QWebSocket *, WebSocketSession*>   ws_sessions;

        /*
         * Connections are handled by worker threads from thread pool.
         */
        QThreadPool                             thread_pool;
};

#endif // ENDPOINTWEBSOCKET_H
