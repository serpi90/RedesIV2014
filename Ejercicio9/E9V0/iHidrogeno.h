#ifndef iHIDROGENO_H
#define	iHIDROGENO_H
#include "Interfaz.h"
#include <string>

class iHidrogeno {
public:
    iHidrogeno(long numero);
    void activar();
    void terminar();
private:
    long numero;
    long id;
    std::string owner;
    Interfaz * interfaz;
};

#endif	/* iHIDROGENO_H */

