#ifndef IPRODUCTOR_H
#define	IPRODUCTOR_H
#include "Interfaz.h"
#include <string>

class iProductor {
public:
    iProductor(long numero);
    void producirOrden(struct orden orden);
private:
    long numero;
    long id;
    std::string owner;
    Interfaz * interfaz;
};

#endif	/* IPRODUCTOR_H */

