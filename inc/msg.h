#ifndef MSG_H
#define MSG_H

#include "../inc/websocketsession.h"
#include <QWebSocket>


struct Msg
{
        enum Id {
                IdMsgUnknown = -1,
                IdMsgHandshakeSyn = 1,
                IdMsgHandshakeAck = 2
        };

        virtual void process() {}

        /**
         * @brief Tx header.
         */
        virtual void post()
        {
                QByteArray raw_msg;
                QDataStream ds(&raw_msg, QIODevice::WriteOnly);

                ds << payload_id;
                ds << payload_len;

                ws_session->bin_msg_tx(raw_msg);
        }

        Msg(Id payload_id, int payload_len, WebSocketSession *ws_session) :
                payload_id(payload_id),
                payload_len(payload_len),
                ws_session(ws_session)
        {}
        virtual ~Msg() {}

        Id payload_id;
        int payload_len;
        WebSocketSession *ws_session;
};

class MsgHandshakeSyn : public Msg {
public:
        MsgHandshakeSyn(int payload_len, WebSocketSession *ws_session) :
                Msg(Id::IdMsgHandshakeSyn, payload_len, ws_session)
        {}
        ~MsgHandshakeSyn() {}

        void process() override;
};

class MsgHandshakeAck : public Msg {
public:
        MsgHandshakeAck(int payload_len, WebSocketSession *ws_session) :
                Msg(Id::IdMsgHandshakeAck, payload_len, ws_session)
        {}
        ~MsgHandshakeAck() {}

        void post() override;
};

#endif // MSG_H
