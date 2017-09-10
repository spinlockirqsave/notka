#include "../inc/msg.h"


QDataStream& operator>>(QDataStream &ds, std::unique_ptr<MsgRX> &msg)
{
        msg->process(ds);
        return ds;
}

void MsgTX::post()
{
        QByteArray raw_msg;
        QDataStream ds(&raw_msg, QIODevice::WriteOnly);

        ds << payload_id;
        ds << payload_len;

        ws_session.bin_msg_tx(raw_msg);
}

void MsgHandshakeSyn::process(QDataStream &ds)
{
        /* As for now, just send ACK after "processing" this SYN. */
        (void) ds;
        MsgHandshakeAck ack(0, ws_session);
        ack.post();
}

void MsgHandshakeAck::post()
{
        /* Just send the header */
        MsgTX::post();
        /* No body for ACK. */
        return;
}

void MsgSaveReq::process(QDataStream &ds)
{
        QString login;
        ds >> login;
}
