#ifndef ISALA_H
#define	ISALA_H

#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"

class iSala {
public:
    iSala(bool abajo);
    void esperarPersona();
private:
    Queue<struct iMessage> * in, * out;
    bool abajo;
    std::string owner;
};

#endif	/* ISALA_H */
