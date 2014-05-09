#include "includes.h"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "Queue.cpp"

int main()
{
    // Create the queues
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_SAMPLER_FROM_INTERFACE, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_SAMPLER_TO_INTERFACE, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_ANALYZER_FROM_INTERFACE, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_ANALYZER_TO_INTERFACE, "iniciador");
    q->create();

    // Create and initialize the shared memory
    SharedMemory<struct sampleHolder> * shm;
    struct sampleHolder * sampleHolder;

    shm = new SharedMemory<struct sampleHolder>(PATH, SHM_SAMPLE_HOLDER, "iniciador");
    shm->create();
    sampleHolder = shm->attach();
    sampleHolder->read = 0;
    sampleHolder->write = 0;
    sampleHolder->amount = 0;
    for (unsigned h = 0; h < HOLDER_CAPACITY; h++)
    {
        sampleHolder->samples[h].id = 0;
        for (unsigned a = 0; a < ANALYZER_AMOUNT; a++)
        {
            sampleHolder->status[h][a] = NOT_ANALYZED;
        }
    }
    for (unsigned a = 0; a < ANALYZER_AMOUNT; a++)
    {
        sampleHolder->analyzerStatus[a] = WORKING;
    }

    // Create and initialize the semaphores
    SemaphoreArray * s;

    s = new SemaphoreArray(PATH, SEM_MUTEX, 1, "iniciador");
    s->create();
    s->post(0);
    s = new SemaphoreArray(PATH, SEM_EMPTY, ANALYZER_AMOUNT, "iniciador");
    s->create();
    s = new SemaphoreArray(PATH, SEM_FULL, 1, "iniciador");
    s->create();

    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        execlp("./comp-sampler", "comp-sampler", NULL);
        perror("Sampler Component - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Sampler Component - fork: ");
    }
    pid = fork();
    if (pid == 0)
    {
        execlp("./comp-analyzer", "comp-analyzer", NULL);
        perror("Analyzer Component - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Analyzer Component - fork: ");
    }

    for (int i = 0; i < SAMPLER_AMOUNT; i++)
    {
        std::stringstream ss;
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./sampler", "sampler", ss.str().c_str(), NULL);
            perror("Sampler - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("Sampler - fork: ");
        }
    }
    for (int i = 0; i < ANALYZER_AMOUNT; i++)
    {
        std::stringstream ss;
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./analyzer", "analyzer", ss.str().c_str(), NULL);
            perror("Analyzer - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("Analyzer - fork: ");
        }
    }
}