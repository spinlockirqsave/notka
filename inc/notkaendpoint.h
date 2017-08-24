#ifndef NOTKAENDPOINT_H
#define NOTKAENDPOINT_H

class NotkaEndPoint {
public:
        virtual int open() = 0;
        virtual void close() = 0;
        virtual ~NotkaEndPoint() {}
};

#endif // NOTKAENDPOINT_H
