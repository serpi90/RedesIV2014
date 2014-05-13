#include "iEmbotelladora.h"
#include "Helper.h"
#include <sstream>

iEmbotelladora::iEmbotelladora(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = M_EMBOTELLADORA + numero;
    ss << "iEmbotelladora " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    in = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    out->get();
}

void iEmbotelladora::termine()
{
    struct iMessage msg;
    msg.type = this->id;
    msg.message = TERMINE;
    out->send(msg);
    msg = in->receive(this->id);
    if (msg.message != CONTINUAR)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL
                << " mensaje incorrecto " << Helper::msgToString(msg.message)
                << " esperaba " << Helper::msgToString(CONTINUAR) << std::endl;
        Helper::output(stderr, ss);
    }
}
