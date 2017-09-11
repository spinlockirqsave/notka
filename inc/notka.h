#ifndef NOTKA_H
#define NOTKA_H


#include <QMainWindow>
#include "../inc/globals.h"

#include "../inc/endpointwebsocket.h"


namespace Ui {
class Notka;
}

#define APP_NAME "Notka"

class Notka : public QMainWindow
{
        Q_OBJECT

public:
        explicit Notka(QWidget *parent = 0);
        Notka(Notka const &) = delete;
        Notka& operator=(const Notka &) = delete;
        ~Notka();

public slots:
        void closeEvent(QCloseEvent *e) override;

private:
        Ui::Notka *ui;
        QList<QSharedPointer<NotkaEndPoint>> endpoints;

        inline void ws_server_add(QWebSocketServer::SslMode mode,
                           QHostAddress address, quint16 port);
        void ws_server_start_stop(bool start);

        /* GUI. */
        void gui_add_ws_server_to_tbw();

private slots:
        /* GUI callbacks. */
        void gui_cb_ws_server_add(bool checked);
        void gui_cb_ws_server_start(bool checked);
        void gui_cb_ws_server_stop(bool checked);
};

#endif // NOTKA_H
