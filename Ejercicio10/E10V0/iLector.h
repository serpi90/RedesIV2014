#ifndef ILECTOR_H
#define	ILECTOR_H
#include "Interfaz.h"
#include <string>

class iLector {
public:
    iLector(long numero);
    double leer();
private:
    long numero;
    long id;
    std::string owner;
    Interfaz * interfaz;
};

#endif	/* ILECTOR_H */

