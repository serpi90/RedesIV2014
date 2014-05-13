#include "iConstruccion.h"
#include "Helper.h"
#include <sstream>

iConstruccion::iConstruccion()
{
    this->owner = "iConstruccion";
    in = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    out->get();
}

void iConstruccion::entregarChasis()
{
    struct iMessage msg;
    msg.type = M_CONSTRUCCION;
    msg.message = ENTREGAR_CHASIS;
    out->send(msg);
    msg = in->receive(M_CONSTRUCCION);
    if (msg.message != ENTREGAR_CHASIS_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(ENTREGAR_CHASIS_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}