#include "Interfaz.h"
#include "Helper.h"

Interfaz::Interfaz(std::string path, int idIn, int idOut, std::string owner)
{
    in = new Queue<struct iMessage>(path, idIn, owner);
    in->get();
    out = new Queue<struct iMessage>(path, idOut, owner);
    out->get();
    this->owner = owner;
}

void Interfaz::send(struct iMessage message)
{
    out->send(message);
}

struct iMessage Interfaz::receive(long mtype)
{
    return in->receive(mtype);
}

struct iMessage Interfaz::request(struct iMessage message, long mtype, enum iMessageType response)
{
    out->send(message);
    message = in->receive(mtype);
    if (message.message != response)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL
                << " mensaje incorrecto " << Helper::msgToString(message.message)
                << " esperaba " << Helper::msgToString(response) << std::endl;
        Helper::output(stderr, ss);
    }
    return message;
}