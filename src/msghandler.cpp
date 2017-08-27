#include "../inc/msghandler.h"


MsgHandler::MsgHandler(QByteArray raw_msg) : raw_msg(raw_msg)
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

        std::unique_ptr<Msg> m(new MsgHandshakeSyn(payload_len));
        return m;
}

#include <QDebug>
void MsgHandler::run()
{
        qDebug() << raw_msg;
        std::unique_ptr<Msg> msg = parse_raw_data(raw_msg);
        if (msg)
                msg->process();
}
