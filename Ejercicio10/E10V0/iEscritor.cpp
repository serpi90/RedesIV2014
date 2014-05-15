#include "iEscritor.h"
#include "includes.h"
#include <sstream>

iEscritor::iEscritor(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = numero * 2 + 1;
    ss << "iOxigeno " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    interfaz = new Interfaz(PATH, Q_TO_INTERFACE, Q_FROM_INTERFACE, this->owner);
}

void iEscritor::escribir(double cotizacion)
{
    struct iMessage msg;
    msg.type = this->id;
    msg.message = QUIERO_ESCRIBIR;
    msg.cotizacion = cotizacion;
    msg = interfaz->request(msg, this->id, ESCRIBI);
}
