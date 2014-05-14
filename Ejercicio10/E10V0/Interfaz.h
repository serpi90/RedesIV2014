#ifndef INTERFAZ_H
#define	INTERFAZ_H

#include "includes.h"
#include "Queue.cpp"
#include <string>

class Interfaz {
public:
    Interfaz(std::string path, int idIn, int idOut, std::string owner);
    void send(struct iMessage message);
    struct iMessage receive(long mtype);
    struct iMessage request(struct iMessage message, long mtype, enum iMessageType response);
private:
    Queue<struct iMessage> * in, * out;
    std::string owner;
};

#endif	/* INTERFAZ_H */

