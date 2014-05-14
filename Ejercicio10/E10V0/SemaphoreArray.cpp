#ifndef SEMAPHOREARRAY_CPP
#define SEMAPHOREARRAY_CPP

#include "SemaphoreArray.h"

SemaphoreArray::SemaphoreArray(std::string path, int identifier, int amount, std::string owner, bool exitOnFailure)
{
    this->owner = owner;
    this->amount = amount;
    this->identifier = identifier;
    this->exitOnFailure = exitOnFailure;

    key = ftok(path.c_str(), identifier);
    if (key == -1)
    {
        error("ftok");
    }
}

void SemaphoreArray::create(void)
{
    semid = semget(key, amount, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (semid == -1)
    {
        error("create");
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } arg;
    arg.val = 0;

    //Initialize all semaphores to 0
    for (int i = 0; i < amount; i++)
    {
        if (semctl(semid, i, SETVAL, arg) == -1)
        {
            error("initialize");
        }
    }
}

void SemaphoreArray::get(void)
{
    semid = semget(key, amount, PERMISSIONS);
    if (semid == -1)
    {
        error("get");
    }
}

void SemaphoreArray::remove(void)
{
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        error("remove");
    }
}

void SemaphoreArray::post(int index)
{
    struct sembuf operation;

    operation.sem_num = index;
    operation.sem_op = 1;
    operation.sem_flg = 0;

    if (semop(semid, &operation, 1) == -1)
    {
        error("post");
    }
}

void SemaphoreArray::wait(int index)
{
    struct sembuf operation;

    operation.sem_num = index;
    operation.sem_op = -1;
    operation.sem_flg = 0;

    if (semop(semid, &operation, 1) == -1)
    {
        error("wait");
    }
}

void SemaphoreArray::error(std::string where)
{
    if (exitOnFailure)
    {
        std::stringstream ss;
        ss << owner << " \033[41m\033[30mError\33[0m semid=" << identifier << " (" << getpid() << ") SEM " << where;
        perror(ss.str().c_str());
        exit(EXIT_FAILURE);
    }
}
#endif
