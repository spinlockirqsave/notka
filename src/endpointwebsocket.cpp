#include "../inc/endpointwebsocket.h"

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QDebug>


EndPointWebSocket::EndPointWebSocket(QWebSocketServer::SslMode mode,
                                     QHostAddress address,
                                     quint16 port,
                                     QObject *parent) :
        QObject(parent),
        ws_server(new QWebSocketServer(QStringLiteral("Notka WebSocket EndPoint"), mode, this)),
        address(address),
        port(port),
        ws_sessions(),
        thread_pool(parent)
{
        connect(ws_server.data(), SIGNAL(newConnection()), this, SLOT(on_new_connection()));
}

EndPointWebSocket::~EndPointWebSocket()
{
        close();
}

int EndPointWebSocket::open()
{
        if (ws_server && ws_server->listen(address, port))
                return 0;
        return -1;
}

void EndPointWebSocket::close()
{
        ws_sessions_disconnect();

        if (ws_server)
                ws_server->close();
}

void EndPointWebSocket::ws_sessions_disconnect()
{
        Q_FOREACH(auto session, ws_sessions)
        {
                // TODO mutex lock and close session
                //session.second->close();
                qDebug() << session;
        }
}

void EndPointWebSocket::on_new_connection()
{
        /* QWebSocketServer owns the socket pointer. */
        QWebSocket *client = ws_server->nextPendingConnection();

        WebSocketSession *ws_session = new WebSocketSession(client);

        /**
         * The &QWebSocket::textMessageReceived
         * and &QWebSocket::binaryMessageReceived signals
         * get connected to ws_session's slots. Only &WebSocket::disconnected
         * signal is handled by this end point because it must remove
         * the session from the container.
         */
        connect(client, &QWebSocket::disconnected, this, &EndPointWebSocket::on_sock_disconnect);

        ws_sessions[client] = ws_session;
}

void EndPointWebSocket::on_sock_disconnect()
{
        auto *client = qobject_cast<QWebSocket *>(sender());

        if (client) {
                auto it = ws_sessions.find(client);

                if (it == ws_sessions.end())
                        return;

                auto ws_session = it.value();

                while(1) {
                        QMutexLocker lock(&ws_session->mutex);
                        if (ws_session->qrunnables_scheduled == 0)
                        {
                                ws_sessions.remove(client);
                                lock.unlock();
                                delete ws_session;
                                return;
                        }
                }
        }
}
