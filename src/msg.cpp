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

        int text_len = payload_len - 32;        /* not including login field */

        if (text_len < 0) {
                /* Nothing to save, error.
                 * TODO: Send error description. */
                return;
        }

        char login_buf[32];
        ds.readRawData(login_buf, sizeof(login_buf));
        QString login(login_buf);

        if (Db::authenticate_user(login, "password") == 0) {
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

void MsgLogin::process(QDataStream &ds)
{
        MsgLoginAck ack(ws_session);

        /* By default: No such user. */
        ws_session.state = NO_SUCH_USER;

        if (payload_len != 64) {
                /* Error, where is login and pass?
                 * TODO: Send error description. */
                ack.post();
                return;
        }

        char login_buf[32];
        ds.readRawData(login_buf, sizeof(login_buf));
        QString login(login_buf);

        char pass_buf[32];
        ds.readRawData(pass_buf, sizeof(pass_buf));
        QString password(pass_buf);

        ws_session.user = login;

        int res = Db::authenticate_user(login, password);
        if (res == 0) {
                ws_session.state = LOGGED_IN;
        } else if (res == 1) {
                /* No such user. */
                ws_session.state = NO_SUCH_USER;
        } else {
                /* Wrong Password. */
                ws_session.state = WRONG_PASSWORD;
        }

        ack.post();

        if (ws_session.state == LOGGED_IN) {
                /* Check if user has any notes and tx them if they have. */
                QByteArray notka;
                if (Db::get_notka(login, notka)) {
                        MsgNotka user_stuff(ws_session, notka);
                        user_stuff.post();
                }
        }
}

void MsgLoginAck::post()
{
        /* Append the header */
        QByteArray raw_msg;
        QDataStream ds(&raw_msg, QIODevice::WriteOnly);

        ds << payload_id;
        ds << payload_len;

        /* And the error code as body for ACK. */
        uint8_t error_code = ws_session.state;

        ds << error_code;

        /* TX */
        ws_session.bin_msg_tx(raw_msg);

        return;
}

void MsgNotka::post()
{
        /* Append the header */
        QByteArray raw_msg;
        QDataStream ds(&raw_msg, QIODevice::WriteOnly);

        ds << payload_id;
        /* No need to writ ethe payload_len as the ds << QByteArray
         * serialises it's size. */
        /* ds << payload_len; */

        /* And the user's notes size (4 bytes) and text as the body. */
        ds << notka;

        /* TX */
        ws_session.bin_msg_tx(raw_msg);

        return;
}
