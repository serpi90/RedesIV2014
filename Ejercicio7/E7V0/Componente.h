#ifndef COMPONENTE_H
#define	COMPONENTE_H

#include "Queue.cpp"
#include "Semaphore.h"
#include "includes.h"
#include <string>

class Componente {
public:
    Componente();
    void init(void);
private:
    std::string owner;
    Queue<struct iMessage> * in, * out, * fromRep, * toRep;
    Semaphore
    * pintar,
    * pintar2,
    * secar,
    * secar2,
    * motor,
    * motor2,
    * interior,
    * interior2,
    * exterior,
    * exterior2;

    void entregarChasis();
    void esperarChasis();
    void recibidoChasis();

    void entregarPintado();
    void esperarPintado();
    void recibidoPintado();

    void entregarSecado();
    void esperarSecado();
    void recibidoSecado();

    void entregarMotor();
    void esperarMotor();
    void recibidoMotor();

    void entregarInteriores();
    void esperarInteriores();
    void recibidoInteriores();

    unsigned reponer(long client, enum materiales m);
};

#endif	/* COMPONENTE_H */

