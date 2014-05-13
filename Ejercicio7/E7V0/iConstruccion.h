#ifndef ICONSTRUCCION_H
#define	ICONSTRUCCION_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iConstruccion {
public:
    iConstruccion();
    void entregarChasis();
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
};

#endif	/* ICONSTRUCCION_H */

