/*
 * notka.cpp
 *
 * Copyright(C) 2017, Piotr Gregor <piotrgregor@rsyncme.org>
 *
 * Notka Online Clipboard
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <QCloseEvent>
#include <QMessageBox>

#include <list>

#include "../inc/notka.h"
#include "ui_notka.h"


Notka::Notka(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Notka),
        endpoints(),
        task_list()
{
        ui->setupUi(this);

        /* Add default server. */
        gui_add_ws_server_to_tbw();


        /* Button Add (server). */
        connect(ui->pb_server_add, SIGNAL(clicked(bool)), this, SLOT(gui_cb_ws_server_add(bool)));

        /* Button Start (server). */
        connect(ui->pb_server_start, SIGNAL(clicked(bool)), this, SLOT(gui_cb_ws_server_start(bool)));

        /* Button Stop (server). */
        connect(ui->pb_server_stop, SIGNAL(clicked(bool)), this, SLOT(gui_cb_ws_server_stop(bool)));

        /* Signals. */
        connect(this, SIGNAL(db_reconnect_signal()), this, SLOT(db_reconnect()));

        if (!Db::init_database())
                throw std::runtime_error("Database cannot be opened");

        /* Start tasks. */
        start_db_reconnect_task();
}

Notka::~Notka()
{
        for (auto &task : task_list) {
                task->stop();
        }

        Db::close_database();
        delete ui;
}

void Notka::db_reconnect()
{
        Db::reconnect();
}

void Notka::start_db_reconnect_task()
{
        std::unique_ptr<Task> t {new Db::DbReconnectTask(1000*1, *this)};
        t->run();
        task_list.push_back(std::move(t));
}

void Notka::ws_server_add(QWebSocketServer::SslMode mode,
                          QHostAddress address, quint16 port)
{
        /* Add new Web Socket server. */
        endpoints.append(QSharedPointer<NotkaEndPoint>(
                         new EndPointWebSocket(mode, address, port)));
}

void Notka::ws_server_start_stop(bool start)
{
        int row = ui->tw_servers->currentRow();
        if (row < 0)
                return;

        auto endpoint = endpoints.at(row);

        if (start)
                endpoint->open();
        else
                endpoint->close();
}

void Notka::gui_add_ws_server_to_tbw()
{
        int row = ui->tw_servers->rowCount();

        /* Add row into GUI. */
        ui->tw_servers->model()->insertRow(row);

        QHostAddress ip(QHostAddress::Any);
        quint16 port = 1235;
        ui->tw_servers->setItem(row, 0, new QTableWidgetItem(ip.toString()));
        ui->tw_servers->setItem(row, 1, new QTableWidgetItem(QString::number(port)));
        ui->tw_servers->setItem(row, 2, new QTableWidgetItem("stopped"));

        /* Insert socket into container. */
        ws_server_add(QWebSocketServer::NonSecureMode, ip, port);
}

/* GUI callbacks. */

void Notka::gui_cb_ws_server_add(bool checked)
{
        (void) checked;
        gui_add_ws_server_to_tbw();
}

void Notka::gui_cb_ws_server_start(bool checked)
{
        (void) checked;
        ws_server_start_stop(1);
}
void Notka::gui_cb_ws_server_stop(bool checked)
{
        (void) checked;
        ws_server_start_stop(0);
}

void Notka::closeEvent(QCloseEvent *e)
{
    QMessageBox::StandardButton res = QMessageBox::question(this, APP_NAME,
                                                            tr("Are you sure?\n"),
                                                            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                            QMessageBox::Yes);
    if (res != QMessageBox::Yes) {
        e->ignore();
        return;
    }

    ws_server_start_stop(0);
    e->accept();
}

/* Signals. */
