#ifndef ISAMPLER_H
#define	ISAMPLER_H

#include <string>
#include "Queue.cpp"
#include "includes.h"

class iSampler {
public:
    iSampler(long number);
    void placeSample(struct sample sample);
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
    long id;
};

#endif	/* ISAMPLER_H */

