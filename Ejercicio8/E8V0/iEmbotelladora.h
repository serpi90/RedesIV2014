#ifndef iEmbotelladora_H
#define	iEmbotelladora_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iEmbotelladora {
public:
    iEmbotelladora(long numero);
    void termine();
private:
    long numero;
    long id;
    std::string owner;
    Queue<struct iMessage> * in, * out;
};

#endif	/* iEmbotelladora_H */

