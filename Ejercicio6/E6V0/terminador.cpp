#include "includes.h"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "Queue.cpp"

int main()
{
    // Remove the queues
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_SAMPLER_FROM_INTERFACE, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_SAMPLER_TO_INTERFACE, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_ANALYZER_FROM_INTERFACE, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_ANALYZER_TO_INTERFACE, "terminador", false);
    q->get();
    q->remove();

    // Remove the shared memory
    SharedMemory<struct sampleHolder> * shm;

    shm = new SharedMemory<struct sampleHolder>(PATH, SHM_SAMPLE_HOLDER, "terminador", false);
    shm->get();
    shm->remove();

    // Create and initialize the semaphores
    SemaphoreArray * s;

    s = new SemaphoreArray(PATH, SEM_MUTEX, 1, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_EMPTY, 1, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_FULL, 1, "terminador", false);
    s->get();
    s->remove();
}