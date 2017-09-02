#include "../inc/msg.h"


void Msg::post()
{
        QByteArray raw_msg;
        QDataStream ds(&raw_msg, QIODevice::WriteOnly);

        ds << payload_id;
        ds << payload_len;

        ws_session.bin_msg_tx(raw_msg);
}

void MsgHandshakeSyn::process()
{
        /* As for now, just send ACK after "processing" this SYN. */
        MsgHandshakeAck ack(0, ws_session);
        ack.post();
        return;
}

void MsgHandshakeAck::post()
{
        /* Just send the header */
        Msg::post();
        /* No body for ACK. */
        return;
}
