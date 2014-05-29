#ifndef IENCARGADO_H
#define	IENCARGADO_H
#include "Interfaz.h"
#include <string>

class iEncargado {
public:
    iEncargado(long numero, enum encargados e);
    struct orden consumirOrden(void);
private:
    long numero;
    long id;
    std::string owner;
    Interfaz * interfaz;
    enum encargados e;
};

#endif	/* IENCARGADO_H */

