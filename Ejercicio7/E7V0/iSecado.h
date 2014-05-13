#ifndef ISECADO_H
#define	ISECADO_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iSecado {
public:
    iSecado();
    void esperarPintado();
    void recibidoPintado();
    void entregarSecado();
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
};

#endif	/* ISECADO_H */

