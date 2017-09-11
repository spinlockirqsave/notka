#include <QDebug>

#include "../inc/msghandler.h"


MsgHandler::MsgHandler(QByteArray raw_msg,
                       WebSocketSession &ws_session) :
        raw_msg(raw_msg),
        ws_session(ws_session)
{
}

std::unique_ptr<MsgRX> MsgHandler::demux_raw_msg(QByteArray raw_msg)
{
        int msg_len = raw_msg.length();
        if (msg_len < 8)
                return nullptr;

        QDataStream ds(&raw_msg, QIODevice::ReadOnly);

        ds.setByteOrder(QDataStream::BigEndian);

        int payload_id;
        ds >> payload_id;

        int payload_len;
        ds >> payload_len;

        switch (payload_id)
        {
        case MsgRX::Id::IdMsgHandshakeSyn:
                return std::unique_ptr<MsgRX>(new MsgHandshakeSyn(payload_len, ws_session));

        case MsgRX::IdMsgSaveReq:
                return std::unique_ptr<MsgRX>(new MsgSaveReq(payload_len, ws_session));

        default:
                return nullptr;
        }
}

void MsgHandler::run()
{
        QMutexLocker ml(&ws_session.mutex);

        qDebug() << __func__ << " " << raw_msg;

        std::unique_ptr<MsgRX> msg = demux_raw_msg(raw_msg);

        if (!msg) {
                /* TODO Tx error description. */
        } else {
                /* Process the message. */
                QDataStream ds(&raw_msg, QIODevice::ReadOnly);

                ds.setByteOrder(QDataStream::BigEndian);

                try {
                        ds >> msg;
                } catch (std::exception &e) {
                        qDebug() << "Processing msg failed: " << e.what();
                }
        }

        /**
         * Processing finished, so tell end point logic
         * - session can be removed from endpoint if there
         * is no QRunnables scheduled to process that session.
         */
        ws_session.qrunnables_scheduled--;
}
