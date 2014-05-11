#include "Sampler.h"
#include "Helper.h"
#include <sstream>

Sampler::Sampler()
{
    this->owner = "Sampler Component";
    in = new Queue<struct iMessage>(PATH, Q_SAMPLER_FROM_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_SAMPLER_TO_INTERFACE, owner);
    out->get();
    mutex = new Semaphore(PATH, SEM_MUTEX, owner);
    mutex->get();
    full = new Semaphore(PATH, SEM_FULL, owner);
    full->get();
    empty = new SemaphoreArray(PATH, SEM_EMPTY, ANALYZER_AMOUNT, owner);
    empty->get();
    shm = new SharedMemory<struct sampleHolder>(PATH, SHM_SAMPLE_HOLDER, owner);
    shm->get();
    sampleHolder = shm->attach();
}

void Sampler::init(void)
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        msg = in->receive(M_SAMPLER);
        ss << owner << " Got " << Helper::msgToString(msg.message) << " from " << (msg.sender - M_SAMPLER) << " (" << msg.sender << ")" << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("Sampler Component: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case PLACE_SAMPLE:
                    msg.message = SAMPLE_PLACED;
                    placeSample(msg.data.sample);
                    break;
                default:
                    ss << owner << " \033[41m\033[30mError\33[0m got wrong message: " << Helper::msgToString(msg.message) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            msg.type = msg.sender;
            msg.sender = M_SAMPLER;
            out->send(msg);
            ss << owner << " Sent " << Helper::msgToString(msg.message) << " to " << (msg.type - M_SAMPLER) << " (" << msg.type << ")" << std::endl;
            Helper::output(stderr, ss);

            exit(EXIT_SUCCESS);
        }
    }
}

void Sampler::placeSample(struct sample sample)
{
    std::stringstream ss;
    mutex->wait();
    // Wait if the sample holder is full
    if (sampleHolder->amount == HOLDER_CAPACITY)
    {
        Helper::output(stdout, owner + " sample holder full, waiting\n");
        sampleHolder->waitingSamplers++;
        mutex->post();
        full->wait();
        mutex->wait();
    }
    ss << owner << " placing sample " << sample.id << " in slot " << sampleHolder->write << " there are now " << sampleHolder->amount << "/" << HOLDER_CAPACITY << " samples" << std::endl;
    Helper::output(stdout, ss);
    sampleHolder->samples[sampleHolder->write] = sample;
    for (unsigned a = 0; a < ANALYZER_AMOUNT; a++)
    {
        sampleHolder->status[sampleHolder->write][a] = NOT_ANALYZED;
    }
    sampleHolder->write = (sampleHolder->write + 1) % HOLDER_CAPACITY;
    sampleHolder->amount++;
    ss << owner << " placing sample " << sample.id << " in slot " << sampleHolder->write << " there are now " << sampleHolder->amount << "/" << HOLDER_CAPACITY << " samples" << std::endl;
    Helper::output(stdout, ss);
    // We check if there's an analyzer waiting
    // This is done always, not only when the holder is empty, 
    // because an analyzer might already have analyzed all the previous samples
    // and is waiting for a new sample to analyze.
    for (unsigned a = 0; a < ANALYZER_AMOUNT; a++)
    {
        if (sampleHolder->analyzerStatus[a] == WAITING)
        {
            ss << owner << " Waking up analyzer " << a << std::endl;
            Helper::output(stdout, ss);
            empty->post(a);
            sampleHolder->analyzerStatus[a] = WORKING;
        }
    }
    mutex->post();
}