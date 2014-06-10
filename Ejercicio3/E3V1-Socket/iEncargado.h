#ifndef IENCARGADO_H
#define	IENCARGADO_H
#include "Queue.cpp"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include <string>

class iEncargado {
public:
    iEncargado(enum consummerType tipo);
    struct orden consumirOrden(void);
private:
    long id;
    std::string owner;
    Queue<struct iMessage> * fromNet;
    Queue<struct iMessage> * toNet;
};

#endif	/* IENCARGADO_H */

