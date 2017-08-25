#ifndef ENDPOINTWEBSOCKET_H
#define ENDPOINTWEBSOCKET_H


#include <QObject>
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"

#include "../inc/notkaendpoint.h"


class EndPointWebSocket : public QObject, public NotkaEndPoint
{
        Q_OBJECT
public:
        /*
         * @mode        Web socket security mode (ws/wss).
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
        /* Gracefully close connections */
        void ws_clients_disconnect();

        void on_new_connection();
        void on_text_msg_rx(QString msg);
        void on_bin_msg_rx(QByteArray msg);
        void on_sock_disconnect();

private:
        QScopedPointer<QWebSocketServer>        ws_server;
        QHostAddress                            address;
        quint16                                 port;

        /* QWebSocketServer owns the sockets pointers. */
        QList<QWebSocket*>                      ws_clients;
};

#endif // ENDPOINTWEBSOCKET_H
