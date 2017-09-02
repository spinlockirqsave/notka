#include <QDebug>

#include "../inc/msghandler.h"


MsgHandler::MsgHandler(QByteArray raw_msg,
                       WebSocketSession &ws_session) :
        raw_msg(raw_msg),
        ws_session(ws_session)
{
}

std::unique_ptr<Msg> MsgHandler::parse_raw_data(QByteArray raw_msg)
{
        int msg_len = raw_msg.length();
        if (msg_len < 8)
                return nullptr;

        QDataStream ds(&raw_msg, QIODevice::ReadOnly);
        int payload_id;
        ds >> payload_id;

        int payload_len;
        ds >> payload_len;

        switch (payload_id)
        {
        case Msg::Id::IdMsgHandshakeSyn:
                return std::unique_ptr<Msg>(new MsgHandshakeSyn(payload_len, ws_session));
        default:
                return nullptr;
        }
}

void MsgHandler::run()
{
        QMutexLocker ml(&ws_session.mutex);

        qDebug() << __func__ << " " << raw_msg;

        std::unique_ptr<Msg> msg = parse_raw_data(raw_msg);
        if (msg)
                msg->process();
        /**
         * Processing finished, so tell end point logic
         * - session can be removed from endpoint if there
         * is no QRunnables scheduled to process that session.
         */
        ws_session.qrunnables_scheduled--;
}
