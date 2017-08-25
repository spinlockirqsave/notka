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
        ws_clients()
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
        ws_clients_disconnect();

        if (ws_server)
                ws_server->close();
}

void EndPointWebSocket::ws_clients_disconnect()
{
        Q_FOREACH(auto client, ws_clients)
        {
                client->close();
        }
}

void EndPointWebSocket::on_new_connection()
{
        /* QWebSocketServer owns the socket pointer. */
        QWebSocket *client = ws_server->nextPendingConnection();

        connect(client, &QWebSocket::textMessageReceived, this, &EndPointWebSocket::on_text_msg_rx);
        connect(client, &QWebSocket::binaryMessageReceived, this, &EndPointWebSocket::on_bin_msg_rx);
        connect(client, &QWebSocket::disconnected, this, &EndPointWebSocket::on_sock_disconnect);

        ws_clients.append(client);
}

void EndPointWebSocket::on_text_msg_rx(QString msg)
{
        (void) msg;
        return;
}

void EndPointWebSocket::on_bin_msg_rx(QByteArray msg)
{
        (void) msg;
        return;
}

void EndPointWebSocket::on_sock_disconnect()
{
        auto *client = qobject_cast<QWebSocket *>(sender());

        if (client) {
                ws_clients.removeAll(client);

                /* QWebSocketServer owns the socket pointer.
                 * Tell the server to schedule socket for deletion */
                client->deleteLater();
        }
}
