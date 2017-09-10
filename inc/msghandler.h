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
         * @brief Implements factory pattern to produce messages derived from Msg struct.
         * @param raw_msg
         * @return Unique pointer to base struct Msg.
         */
        std::unique_ptr<MsgRX> parse_raw_msg(QByteArray raw_msg);

        inline void run() override;
private:
        QByteArray              raw_msg;
        WebSocketSession        &ws_session;
};

#endif // MSGHANDLER_H
