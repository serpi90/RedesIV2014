#ifndef ISECADO_H
#define	ISECADO_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iRepositor {
public:
    iRepositor();
    enum materiales esperarPedido();
    void responderPedido(unsigned cantidad);
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
    struct iMessage lastMessage;
};

#endif	/* ISECADO_H */

