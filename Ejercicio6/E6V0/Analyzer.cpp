#include "Analyzer.h"
#include "Helper.h"
#include <sstream>

Analyzer::Analyzer()
{
    this->owner = "Analyzer";
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
    std::string owner = "Componente " + this->owner;
    while (true)
    {
        msg = in->receive(M_ANALYZER);
        ss << owner << " recibi " << msg.message << " de " << (msg.sender - M_ANALYZER) << " (" << msg.sender << ")" << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("Componente Analyzer: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case GET_SAMPLE:
                    msg.message = GOT_SAMPLE;
                    msg.data.sample = getSample(msg.data.number);
                    break;
                default:
                    ss << owner << " \033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            msg.type = msg.sender;
            msg.sender = M_ANALYZER;
            out->send(msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << " a " << (msg.type - M_ANALYZER) << " (" << msg.type << ")" << std::endl;
            Helper::output(stderr, ss);

            exit(EXIT_SUCCESS);
        }
    }
}

struct sample Analyzer::getSample(long number)
{
    std::stringstream ss;
    ss << owner << " " << number;
    std::string owner = ss.str();
    ss.str("");
    Semaphore * empty = new Semaphore(this->empty, number);

    mutex->wait();
    unsigned sampleNr, idx;
    // Find the index of the first sample not analyzed.
    for (sampleNr = 0, idx = (sampleHolder->read + sampleNr) % HOLDER_CAPACITY; sampleNr < sampleHolder->amount && sampleHolder->status[idx][number] != NOT_ANALYZED; sampleNr++);
    if (sampleHolder->status[idx][number] != NOT_ANALYZED)
    {
        sampleHolder->analyzerStatus[number] = WAITING;
        Helper::output(stdout, owner + " espero muestras para analizar\n");
        mutex->post();
        empty->wait();
        mutex->wait();
        // Find the index of the first sample not analyzed. (Again)
        for (sampleNr = 0, idx = (sampleHolder->read + sampleNr) % HOLDER_CAPACITY; sampleNr < sampleHolder->amount && sampleHolder->status[idx][number] != NOT_ANALYZED; sampleNr++);
    }
    sampleHolder->analyzerStatus[number] = WORKING;
    sampleHolder->status[idx][number] = ANALYZED;
    mutex->post();
    return sampleHolder->samples[0];
}