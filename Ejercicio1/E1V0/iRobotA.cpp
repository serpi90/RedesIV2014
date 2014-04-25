/* 
 * File:   iRobotA.cpp
 * Author: Julian Maestri
 * 
 * Created on April 6, 2014, 1:34 PM
 */

#include "iRobotA.h"

iRobotA::iRobotA(int id, std::string owner, bool first)
{
    this->number = id;
    this->owner = owner;
    fromComponent = new Queue(PATH, Q_PLAT_TO_INTERFACE, owner);
    toComponent = new Queue(PATH, Q_PLAT_FROM_INTERFACE, owner);

    if (first)
    {
        fromComponent->create();
        toComponent->create();
        SemaphoreArray * sem;
        SharedMemory * shm;
        struct plate * plate;
        sem = new SemaphoreArray(PATH, SEM_MUTEX, 1, owner);
        sem->create();
        sem->post(0);
        sem = new SemaphoreArray(PATH, SEM_SEMA, ROBOT_AMOUNT, owner);
        sem->create();
        sem = new SemaphoreArray(PATH, SEM_SEMB, ROBOT_AMOUNT, owner);
        sem->create();
        sem = new SemaphoreArray(PATH, SEM_SEME, ROBOT_AMOUNT, owner);
        sem->create();
        shm = new SharedMemory(PATH, SHM_PLAT, sizeof (struct plate), owner);
        shm->create();
        plate = (struct plate*) shm->attach();
        plate->activeRobotsA = ROBOT_AMOUNT;
        plate->activeRobotsB = ROBOT_AMOUNT;
        plate->storedAmount = 0;
        for (int i = 0; i < ROBOT_AMOUNT; i++)
        {
            plate->statusA[i] = WORKING;
            plate->statusB[i] = IDLE;
            plate->statusE[i] = NOT_WAITING;
        }
        for (int i = 0; i < PLAT_SIZE; i++)
        {
            plate->slots[i].state = EMPTY;
        }
        shm->dettach();
    } else
    {
        fromComponent->get();
        toComponent->get();
    }
    std::stringstream ss;
    ss << number;
    pid_t child = fork();
    if (child == 0)
    {
        execlp("./plataforma", "Plataforma", "a", ss.str().c_str(), NULL);
        perror("Plataforma - execlp: ");
        exit(EXIT_FAILURE);
    } else if (child < 0)
    {
        perror("Plataforma - fork: ");
    }
}

int iRobotA::reservar(void)
{
    iMessage buf;
    buf.function = RESERVE;
    buf.senderType = M_ROBOTA + number;
    buf.type = M_PLATA + number;
    buf.robot = number;
    std::stringstream ss;
    ss << owner << ": Enviando " << msgToStr(buf.function) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
    if (write(fileno(stdout), ss.str().c_str(), ss.str().size()))
    {
    }
    toComponent->send((void*) &buf, sizeof (buf));
    fromComponent->receive((void*) &buf, sizeof (buf), M_ROBOTA + number);
    if (buf.function != RESERVE_OK)
    {
        ss.str("");
        ss << owner << ": \033[41m\033[30mError\33[0m al reservar no recibi " << msgToStr(RESERVE_OK) << ": (" << msgToStr(buf.function) << ")" << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
        if (write(fileno(stderr), ss.str().c_str(), ss.str().size()))
        {
        }
    }
    return buf.content.location;
}

void iRobotA::poner(int location)
{
    iMessage buf;
    buf.function = PUT;
    buf.senderType = M_ROBOTA + number;
    buf.type = M_PLATA + number;
    buf.robot = number;
    buf.content.putMessage.location = location;
    buf.content.putMessage.idDispositive = 666;
    std::stringstream ss;
    ss << owner << ": Enviando " << msgToStr(buf.function) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
    if (write(fileno(stdout), ss.str().c_str(), ss.str().size()))
    {
    }
    toComponent->send((void*) &buf, sizeof (buf));
    fromComponent->receive((void*) &buf, sizeof (buf), M_ROBOTA + number);
    if (buf.function != PUT_OK)
    {
        ss.str("");
        ss << owner << ": \033[41m\033[30mError\33[0m al poner no recibi " << msgToStr(PUT_OK) << ": (" << msgToStr(buf.function) << ")" << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
        if (write(fileno(stderr), ss.str().c_str(), ss.str().size()))
        {
        }
    }
}

bool iRobotA::avisarApagado()
{
    iMessage buf;
    buf.function = SHUTDOWN;
    buf.content.rtype = 'a';
    buf.senderType = M_ROBOTA + number;
    buf.type = M_PLATA + number;
    buf.robot = number;
    std::stringstream ss;
    ss << owner << ": Enviando " << msgToStr(buf.function) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
    if (write(fileno(stdout), ss.str().c_str(), ss.str().size()))
    {
    }
    toComponent->send((void*) &buf, sizeof (buf));
    fromComponent->receive((void*) &buf, sizeof (buf), M_ROBOTA + number);
    if (buf.function != SHUTDOWN_OK && buf.function != SHUTDOWN_LAST)
    {
        ss.str("");
        ss << owner << ": \033[41m\033[30mError\33[0m al apagar no recibi " << msgToStr(SHUTDOWN_OK) << " : (" << msgToStr(buf.function) << ")" << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
        if (write(fileno(stderr), ss.str().c_str(), ss.str().size()))
        {
        }
    }
    return buf.function == SHUTDOWN_LAST;
}

iRobotA::~iRobotA()
{
    avisarApagado();
}

std::string iRobotA::msgToStr(enum iPlatMessage function)
{
    switch (function)
    {
        case RESERVE:
            return "\033[1;34mRESERVE\033[0m";
        case RESERVE_OK:
            return "\033[44m\033[30mRESERVE_OK\033[0m";
        case PUT:
            return "\033[1;32mPUT\033[0m";
        case PUT_OK:
            return "\033[32mPUT_OK\033[0m";
        case TAKE:
            return "\033[1;31mTAKE\033[0m";
        case TAKE_OK:
            return "\033[31mTAKE_OK\033[0m";
        case TAKE_NONE:
            return "\033[41mTAKE_NONE\033[0m";
        case QUERY_ROBOTS_OR_DISPOSITIVES:
            return "\033[1;33mQUERY_ROBOTS_OR_DISPOSITIVES\033[0m";
        case QUERY_ROBOTS_OR_DISPOSITIVES_YES:
            return "\033[33mQUERY_ROBOTS_OR_DISPOSITIVES_YES\033[0m";
        case QUERY_ROBOTS_OR_DISPOSITIVES_NO:
            return "\033[33mQUERY_ROBOTS_OR_DISPOSITIVES_NO\033[0m";
        case SHUTDOWN:
            return "\033[35mSHUTDOWN\033[0m";
        case SHUTDOWN_OK:
            return "\033[45m\033[30mSHUTDOWN_OK\033[0m";
        default:
            std::stringstream ss;
            ss << "\033[47" << (int) function << "\033[0m";
            return ss.str();
    }
}
