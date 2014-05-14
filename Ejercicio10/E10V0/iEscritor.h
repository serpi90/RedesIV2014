#ifndef IESCRITOR_H
#define	IESCRITOR_H
#include "Interfaz.h"
#include <string>

class iEscritor {
public:
    iEscritor(long numero);
    void escribir(double);
private:
    long numero;
    long id;
    std::string owner;
    Interfaz * interfaz;
};

#endif	/* IESCRITOR_H */

