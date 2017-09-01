#include "../inc/msg.h"


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
