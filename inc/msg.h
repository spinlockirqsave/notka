#ifndef MSG_H
#define MSG_H


struct Msg
{
        enum class Id {
                MsgUnknown = -1,
                MsgHandshakeSyn = 1,
                MsgHandshakeAck = 2
        };

        virtual void process() = 0;
        virtual void post() = 0;

        Msg(Id payload_id, int payload_len) :
                payload_id(payload_id),
                payload_len(payload_len)
        {}
        virtual ~Msg() {}

        Id payload_id;
        int payload_len;
};

class MsgHandshakeSyn : public Msg {
public:
        MsgHandshakeSyn(int payload_len) :
                Msg(Id::MsgHandshakeSyn, payload_len)
        {}
        ~MsgHandshakeSyn() {}

        void process() override;
        void post() override;
};

#endif // MSG_H
