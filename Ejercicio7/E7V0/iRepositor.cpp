#include "iRepositor.h"
#include "Helper.h"
#include <sstream>

iRepositor::iRepositor()
{
    this->owner = "iRepositor";
    in = new Queue<struct iMessage>(PATH, Q_TO_IREPOSITOR, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_IREPOSITOR, owner);
    out->get();
}

enum materiales iRepositor::esperarPedido()
{
    std::stringstream ss;
    lastMessage = in->receive(0);
    if (lastMessage.message != REPONER)
    {
        ss << owner << BG_RED << WHITE << " Error" << NORMAL
                << " mensaje incorrecto " << Helper::msgToString(lastMessage.message)
                << " de " << Helper::msgToString(lastMessage.type)
                << " esperaba " << Helper::msgToString(REPONER) << std::endl;
        Helper::output(stderr, ss);
    }
    return lastMessage.data.aReponer;
}

void iRepositor::responderPedido(unsigned cantidad)
{
    struct iMessage response;
    response.type = lastMessage.type;
    response.message = REPONER;
    response.data.cantidad = cantidad;
    out->send(response);
}