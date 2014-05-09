#ifndef ANALYZER_H
#define	ANALYZER_H

#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"
#include "includes.h"
#include <string>

class Analyzer {
public:
    Analyzer();
    void init(void);
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
    Semaphore * mutex, * full;
    SemaphoreArray * empty;
    SharedMemory<struct sampleHolder> * shm;
    struct sampleHolder * sampleHolder;

    struct sample getSample(long number);
};

#endif	/* ANALYZER_H */

