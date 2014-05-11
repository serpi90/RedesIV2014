#include "includes.h"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "Queue.cpp"

int main()
{
    pid_t pid;
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