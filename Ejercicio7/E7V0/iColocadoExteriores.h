#ifndef ICOLOCADOEXTERIORES_H
#define	ICOLOCADOEXTERIORES_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iColocadoExteriores {
public:
    iColocadoExteriores();
    void esperarInteriores();
    void recibidoInteriores();
    unsigned reponerPuertas();
    unsigned reponerCapos();
    unsigned reponerBaules();

private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
};

#endif	/* ICOLOCADOEXTERIORES_H */

