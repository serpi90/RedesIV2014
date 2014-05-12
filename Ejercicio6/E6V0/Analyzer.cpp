#include "Analyzer.h"
#include "Helper.h"
#include <sstream>

Analyzer::Analyzer()
{
    this->owner = "Analyzer component";
    in = new Queue<struct iMessage>(PATH, Q_ANALYZER_FROM_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_ANALYZER_TO_INTERFACE, owner);
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

void Analyzer::init(void)
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        msg = in->receive(M_ANALYZER);
        ss << owner << " got " << Helper::msgToString(msg.message) << " from " << (msg.sender - M_ANALYZER) << " (" << msg.sender << ")" << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("Analyzer component: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case GET_SAMPLE:
                    msg.message = GOT_SAMPLE;
                    msg.data.sample = getSample(msg.data.number);
                    break;
                default:
                    ss << owner << " \033[41m\033[30mError\33[0m got wrong message " << Helper::msgToString(msg.message) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            msg.type = msg.sender;
            msg.sender = M_ANALYZER;
            out->send(msg);
            ss << owner << " sent " << Helper::msgToString(msg.message) << " to " << (msg.type - M_ANALYZER) << " (" << msg.type << ")" << std::endl;
            Helper::output(stderr, ss);

            exit(EXIT_SUCCESS);
        }
    }
}

struct sample Analyzer::getSample(long number)
{
    struct sample sample;
    std::stringstream ss;
    ss << owner << " " << number;
    std::string owner = ss.str();
    ss.str("");
    Semaphore * empty = new Semaphore(this->empty, number);

    mutex->wait();
    unsigned sampleIndex, i;
    i = 0;
    // Find the index of the first sample not analyzed.
    do
    {
        sampleIndex = (sampleHolder->read + i) % HOLDER_CAPACITY;
    } while (i++ < sampleHolder->amount && sampleHolder->status[sampleIndex][number] != NOT_ANALYZED);
    if (sampleHolder->status[sampleIndex][number] != NOT_ANALYZED)
    {
        sampleHolder->analyzerStatus[number] = WAITING;
        Helper::output(stdout, owner + " waiting for samples to analyze\n");
        mutex->post();
        empty->wait();
        mutex->wait();
        // Find the index of the first sample not analyzed. (Again)
        // We don't reset i to 0 because we know all the previous samples are analyzed
        // and i points to the place after where the new sample would be.
        i--;
        do
        {
            sampleIndex = (sampleHolder->read + i) % HOLDER_CAPACITY;
        } while (i++ < sampleHolder->amount && sampleHolder->status[sampleIndex][number] != NOT_ANALYZED);
    }
    ss << owner << " analyzing sample " << sampleHolder->samples[sampleIndex].id << " in slot " << sampleIndex << std::endl;
    Helper::output(stdout, ss);
    sampleHolder->analyzerStatus[number] = WORKING;
    sampleHolder->status[sampleIndex][number] = ANALYZED;
    sample = sampleHolder->samples[sampleIndex];
    // If it's the first element then it can be removed if everyone read it
    if (sampleIndex == sampleHolder->read)
    {
        unsigned a = 0;
        while (a < ANALYZER_AMOUNT && sampleHolder->status[sampleIndex][a++] == ANALYZED);
        // If everyone read it remove the element
        if (a == ANALYZER_AMOUNT)
        {
            ss << owner << " removing sample " << sampleHolder->samples[sampleIndex].id << " from slot " << sampleIndex << std::endl;
            Helper::output(stdout, ss, "\033[1;31m");
            sampleHolder->samples[sampleIndex].id = 0;
            sampleHolder->read = (sampleHolder->read + 1) % HOLDER_CAPACITY;
            sampleHolder->amount--;
            // If any sampler is waiting, notify the samplers.
            if (sampleHolder->waitingSamplers)
            {
                ss << owner << " waking up a sampler" << std::endl;
                Helper::output(stdout, ss, "\033[1;32m");
                sampleHolder->waitingSamplers--;
                full->post();
            }
        }
    }
    mutex->post();
    return sample;
}
