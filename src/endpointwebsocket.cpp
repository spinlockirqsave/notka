#include "../inc/endpointwebsocket.h"

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"


EndPointWebSocket::EndPointWebSocket(QWebSocketServer::SslMode mode,
                                     QObject *parent) :
        QObject(parent),
        ws_server(new QWebSocketServer(QStringLiteral("Notka WebSocket EndPoint"), mode, this)),
        ws_clients()
{
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
        return;
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
        return;
}
