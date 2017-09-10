#ifndef MSG_H
#define MSG_H

#include "../inc/websocketsession.h"
#include <QWebSocket>

#include <memory>

class MsgRX;

/**
 * @brief Make the "ds >> msg" notation possible.
 * @param ds    The data stream to be read.
 * @param msg   The message to be initialised from the stream @ds.
 */
QDataStream& operator>>(QDataStream &ds, std::unique_ptr<MsgRX> &msg);

struct MsgRX
{
        enum Id {
                IdMsgUnknown = -1,
                IdMsgHandshakeSyn = 1,
                IdMsgSaveReq = 2
        };

        virtual void process(QDataStream &ds) = 0;

        explicit MsgRX(Id payload_id, int payload_len, WebSocketSession &ws_session) :
                payload_id(payload_id),
                payload_len(payload_len),
                ws_session(ws_session)
        {}
        virtual ~MsgRX() {}
        MsgRX(const MsgRX&) = delete;
        MsgRX& operator=(const MsgRX&) = delete;

        Id                      payload_id;
        int                     payload_len;
        WebSocketSession        &ws_session;
};

struct MsgTX
{
        enum Id {
                IdMsgUnknown = -1,
                IdMsgHandshakeAck = 1
        };

        /**
         * @brief Tx header.
         */
        virtual void post();

        explicit MsgTX(Id payload_id, int payload_len, WebSocketSession &ws_session) :
                payload_id(payload_id),
                payload_len(payload_len),
                ws_session(ws_session)
        {}
        virtual ~MsgTX() {}

        Id                      payload_id;
        int                     payload_len;
        WebSocketSession        &ws_session;
};

class MsgHandshakeSyn : public MsgRX {
public:
        explicit MsgHandshakeSyn(int payload_len, WebSocketSession &ws_session) :
                MsgRX(Id::IdMsgHandshakeSyn, payload_len, ws_session)
        {}
        ~MsgHandshakeSyn() {}
        void process(QDataStream &ds) override;
};

class MsgHandshakeAck : public MsgTX {
public:
        explicit MsgHandshakeAck(int payload_len, WebSocketSession &ws_session) :
                MsgTX(Id::IdMsgHandshakeAck, payload_len, ws_session)
        {}
        ~MsgHandshakeAck() {}
        void post() override;
};

class MsgSaveReq : public MsgRX {
public:
        explicit MsgSaveReq(int payload_len, WebSocketSession &ws_session) :
                MsgRX(Id::IdMsgSaveReq, payload_len, ws_session)
        {}
        ~MsgSaveReq() {}
        MsgSaveReq(const MsgSaveReq&) = delete;
        MsgSaveReq& operator=(const MsgSaveReq&) = delete;
        void process(QDataStream &ds) override;
};

#endif // MSG_H
