#ifndef ICOLOCADOINTERIORES_H
#define	ICOLOCADOINTERIORES_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iColocadoInteriores {
public:
    iColocadoInteriores();
    void esperarMotor();
    void recibidoMotor();
    void entregarInteriores();
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
};

#endif	/* ICOLOCADOINTERIORES_H */

