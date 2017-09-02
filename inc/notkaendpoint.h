#ifndef NOTKAENDPOINT_H
#define NOTKAENDPOINT_H


/**
 * @brief Interface implemented by communication end points.
 */
class NotkaEndPoint {
public:
        virtual int open() = 0;
        virtual void close() = 0;
        virtual ~NotkaEndPoint() {}
};

#endif // NOTKAENDPOINT_H
