#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <QRunnable>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QScopedPointer>

#include <memory>

#include "../inc/msg.h"


class MsgHandler : public QRunnable
{
public:
        MsgHandler(QByteArray raw_msg);
        std::unique_ptr<Msg> parse_raw_data(QByteArray raw_msg);
        inline void run() override;
private:
        QByteArray      raw_msg;
};

#endif // MSGHANDLER_H
