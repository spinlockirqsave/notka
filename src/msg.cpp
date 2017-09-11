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
        int id;
        ds >> id;

        int payload_len;
        ds >> payload_len;

        int text_len = payload_len - 32;

        if (text_len < 0) {
                /* Nothing to save, error.
                 * TODO: Send error description. */
                return;
        }

        char login_buf[32];
        ds.readRawData(login_buf, sizeof(login_buf));
        QString login(login_buf);

        if (Db::authenticate_user(login, "password")) {
                char* bytes = new char[text_len];
                ds.readRawData(bytes, text_len);
                QByteArray notka = QByteArray::fromRawData(bytes, text_len);

                bool ok = Db::save_notka(login, notka);

                if (!ok) {
                        /* TODO: send error description. */
                        return;
                }
                /* TODO: tx error OK. */
        }
        /* TODO: tx error description. */
}
