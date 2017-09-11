#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <QRunnable>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QScopedPointer>

#include <memory>

#include "../inc/msg.h"
#include "../inc/websocketsession.h"


class MsgHandler : public QRunnable
{
public:
        explicit MsgHandler(QByteArray raw_msg, WebSocketSession &ws_session);

        /**
         * @brief       Implements factory pattern to produce high level RX messages
         *              (derived from MsgRX struct) by demuxing raw binary messages
         *              based on payload id.
         * @param raw_msg       Binary stream to parse.
         * @return      Unique pointer to base struct MsgRX.
         */
        std::unique_ptr<MsgRX> demux_raw_msg(QByteArray raw_msg);

        inline void run() override;
private:
        QByteArray              raw_msg;
        WebSocketSession        &ws_session;
public:
        std::unique_ptr<QDataStream>    ds;
};

#endif // MSGHANDLER_H
