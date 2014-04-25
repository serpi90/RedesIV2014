#ifndef SHAREDMEMORY_CPP
#define SHAREDMEMORY_CPP

#include "SharedMemory.h"

SharedMemory::SharedMemory(std::string path, int identifier, unsigned int size, std::string owner, bool exit)
{
    this->owner = owner;
    this->attached = false;
    this->size = size;
    this->identifier = identifier;
    this->exitOnFailure = exit;

    key = ftok(path.c_str(), identifier);
    if (key == -1)
    {
        error("ftok");
    }
}

SharedMemory::~SharedMemory()
{
    dettach();
}

void SharedMemory::create()
{
    shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (shmid == -1)
    {
        error("create");
    }
}

void SharedMemory::get(void)
{
    shmid = shmget(key, size, PERMISSIONS);
    if (shmid == -1)
    {
        error("get");
    }
}

void SharedMemory::remove(void)
{
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        error("remove");
    }
}

void * SharedMemory::attach()
{
    if (!attached)
    {
        attachAddress = shmat(shmid, NULL, PERMISSIONS);
        if (attachAddress == (void *) - 1)
        {
            error("attach");
        }
        attached = true;
    }
    return attachAddress;
}

void SharedMemory::dettach()
{
    if (attached)
    {
        if (shmdt(attachAddress) == -1)
        {
            error("dettach");
        }
        attached = false;
    }
}

void SharedMemory::error(std::string where)
{
    if (exitOnFailure)
    {
        std::stringstream ss;
        ss << owner << " \033[41m\033[30mError\33[0m shmid=" << identifier << " (" << getpid() << ") SHM " << where;
        perror(ss.str().c_str());
        exit(EXIT_FAILURE);
    }
}

#endif
