#ifndef IENCARGADO_H
#define	IENCARGADO_H
#include "Queue.cpp"
#include <string>

class iEncargado {
public:
    iEncargado(long numero);
    struct orden consumirOrden(void);
private:
    long numero;
    long id;
    std::string owner;
    Queue<struct msgAlmacen> * q;
};

#endif	/* IENCARGADO_H */

