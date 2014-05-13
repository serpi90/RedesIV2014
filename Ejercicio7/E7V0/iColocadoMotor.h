#ifndef ICOLOCADOMOTOR_H
#define	ICOLOCADOMOTOR_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iColocadoMotor {
public:
    iColocadoMotor();
    void esperarSecado();
    void recibidoSecado();
    void entregarMotor();
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
};

#endif	/* ICOLOCADOMOTOR_H */

