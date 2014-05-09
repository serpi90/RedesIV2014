#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "SemaphoreArray.h"

class Semaphore {
private:
    SemaphoreArray * semArr;
    int index;
public:
    Semaphore(std::string path, int identifier, std::string owner, bool exitOnFailure = true);
    Semaphore(SemaphoreArray * semArr, int index);
    void create();
    void get();
    void remove();
    void wait();
    void post();
};

#endif
