#include "Helper.h"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "includes.h"

#include <string>
#include <sstream>
#include <iomanip>

#define WIDTH 7

int main()
{
    std::string owner = "shmstat";
    Semaphore * mutex = new Semaphore(PATH, SEM_MUTEX, owner);
    mutex->get();
    SharedMemory <struct sampleHolder> * shm = new SharedMemory<struct sampleHolder>(PATH, SHM_SAMPLE_HOLDER, owner);
    shm->get();
    struct sampleHolder copy, * s = shm->attach();
    std::stringstream ss;
    mutex->wait();
    copy = *s;
    mutex->post();

    ss << "+------------+---------+" << std::endl;
    ss << "| Samples    | " << std::setw(WIDTH) << copy.amount << " |" << std::endl;
    ss << "| Read       | " << std::setw(WIDTH) << copy.read << " |" << std::endl;
    ss << "| Write      | " << std::setw(WIDTH) << copy.write << " |" << std::endl;
    ss << "| Waiting    | " << std::setw(WIDTH) << copy.waitingSamplers << " |" << std::endl;
    for (unsigned i = 0; i < HOLDER_CAPACITY; i++)
    {
        ss << "| Sample " << std::setw(3) << i << " | " << std::setw(WIDTH) << copy.samples[i].id << " |" << std::endl;
    }
    for (unsigned a = 0; a < ANALYZER_AMOUNT; a++)
    {
        ss << "| Analyzer " << a << " | " << std::setw(WIDTH) << Helper::msgToString(copy.analyzerStatus[a]) << " |" << std::endl;
    }
    ss << "+------------+---------+" << std::endl;
    Helper::output(stdout, ss);
    return 0;
}

