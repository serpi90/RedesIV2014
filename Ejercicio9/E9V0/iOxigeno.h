#ifndef IOXIGENO_H
#define	IOXIGENO_H
#include "Interfaz.h"
#include <string>

class iOxigeno {
public:
    iOxigeno(long numero);
    void activar();
    void terminar();
private:
    long numero;
    long id;
    std::string owner;
    Interfaz * interfaz;
};

#endif	/* IOXIGENO_H */

