#include "../inc/websocketsession.h"


QThreadPool WebSocketSession::thread_pool;

WebSocketSession::~WebSocketSession()
{
        ws->close();
        ws->deleteLater();
}

void WebSocketSession::on_text_msg_rx(QString msg)
{
        QMutexLocker ml(&mutex);
        (void) msg;
}

void WebSocketSession::on_bin_msg_rx(QByteArray raw_msg)
{
        QMutexLocker ml(&mutex);
        MsgHandler *mh;

        try {
                mh = new MsgHandler(raw_msg, this);

                qrunnables_scheduled++;
                ml.unlock();

                mh->setAutoDelete(true);
                thread_pool.start(mh);
        } catch (...) {
                // TODO handle error
        }

        return;
}
