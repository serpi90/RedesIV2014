#ifndef SEMAPHORE_CPP
#define SEMAPHORE_CPP

#include "Semaphore.h"

Semaphore::Semaphore(std::string path, int identifier, std::string owner, bool exitOnFailure)
{
    this->index = 0;
    semArr = new SemaphoreArray(path, identifier, 1, owner, exitOnFailure);
}

Semaphore::Semaphore(SemaphoreArray * semArr, int index)
{
    this->index = index;
    this->semArr = semArr;
}

void Semaphore::create(void)
{
    semArr->create();
}

void Semaphore::get(void)
{
    semArr->get();
}

void Semaphore::remove(void)
{
    semArr->remove();
}

void Semaphore::post(void)
{
    semArr->post(index);

}

void Semaphore::wait(void)
{
    semArr->wait(index);
}

#endif
