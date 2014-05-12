#ifndef SAMPLER_H
#define	SAMPLER_H

#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"
#include "includes.h"
#include <string>

class Sampler {
public:
    Sampler();
    void init(void);
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
    Semaphore * mutex;
    Semaphore * full;
    Semaphore * mutexSampler;
    SemaphoreArray * empty;
    SharedMemory<struct sampleHolder> * shm;
    struct sampleHolder * sampleHolder;

    void placeSample(struct sample);
};

#endif	/* SAMPLER_H */

