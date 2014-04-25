/* 
 * File:   iRobotB.cpp
 * Author: Julian Maestri
 * 
 * Created on April 6, 2014, 1:34 PM
 */

#include "iRobotB.h"

iRobotB::iRobotB(int id, std::string owner)
{
    this->number = id;
    this->owner = owner;
    fromComponent = new Queue(PATH, Q_PLAT_TO_INTERFACE, owner);
    toComponent = new Queue(PATH, Q_PLAT_FROM_INTERFACE, owner);
    fromComponent->get();
    toComponent->get();
    std::stringstream ss;
    ss << number;
    pid_t child = fork();
    if (child == 0)
    {
        execlp("./plataforma", "Plataforma", "b", ss.str().c_str(), NULL);
        perror("Plataforma - execlp: ");
        exit(EXIT_FAILURE);
    } else if (child < 0)
    {
        perror("Plataforma - fork: ");
    }
}

bool iRobotB::sacar(void)
{
    iMessage buf;
    buf.function = TAKE;
    buf.senderType = M_ROBOTB + number;
    buf.type = M_PLATB + number;
    buf.robot = number;
    std::stringstream ss;
    ss << owner << ": Enviando " << msgToStr(buf.function) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
    if (write(fileno(stdout), ss.str().c_str(), ss.str().size()))
    {
    }
    toComponent->send((void*) &buf, sizeof (buf));
    fromComponent->receive((void*) &buf, sizeof (buf), M_ROBOTB + number);
    if (buf.function != TAKE_OK && buf.function != TAKE_NONE)
    {
        ss.str("");
        ss << owner << ": \033[41m\033[30mError\33[0m al sacar no recibi " << msgToStr(TAKE_OK) << " o " << msgToStr(TAKE_NONE) << ": (" << msgToStr(buf.function) << ")" << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
        if (write(fileno(stderr), ss.str().c_str(), ss.str().size()))
        {
        }
    }
    return buf.function == TAKE_OK;
}

bool iRobotB::hayDispositivosORobots(void)
{
    iMessage buf;
    buf.function = QUERY_ROBOTS_OR_DISPOSITIVES;
    buf.senderType = M_ROBOTB + number;
    buf.type = M_PLATB + number;
    buf.robot = number;
    std::stringstream ss;
    ss << owner << ": Enviando " << msgToStr(buf.function) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
    if (write(fileno(stdout), ss.str().c_str(), ss.str().size()))
    {
    }
    toComponent->send((void*) &buf, sizeof (buf));
    fromComponent->receive((void*) &buf, sizeof (buf), M_ROBOTB + number);
    if (buf.function != QUERY_ROBOTS_OR_DISPOSITIVES_YES && buf.function != QUERY_ROBOTS_OR_DISPOSITIVES_NO)
    {
        ss.str("");
        ss << owner << ": \033[41m\033[30mError\33[0m al preguntar no recibi " << msgToStr(QUERY_ROBOTS_OR_DISPOSITIVES_YES) << "o" << msgToStr(QUERY_ROBOTS_OR_DISPOSITIVES_NO) << ": (" << msgToStr(buf.function) << ")" << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
        if (write(fileno(stderr), ss.str().c_str(), ss.str().size()))
        {
        }
    }
    return buf.function == QUERY_ROBOTS_OR_DISPOSITIVES_YES;
}

bool iRobotB::avisarApagado()
{
    iMessage buf;
    buf.function = SHUTDOWN;
    buf.content.rtype = 'b';
    buf.senderType = M_ROBOTB + number;
    buf.type = M_PLATB + number;
    buf.robot = number;
    std::stringstream ss;
    ss << owner << ": Enviando " << msgToStr(buf.function) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
    if (write(fileno(stdout), ss.str().c_str(), ss.str().size()))
    {
    }
    toComponent->send((void*) &buf, sizeof (buf));
    fromComponent->receive((void*) &buf, sizeof (buf), M_ROBOTB + number);
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

iRobotB::~iRobotB()
{
    if (avisarApagado())
    {
        SemaphoreArray * s;
        SharedMemory * shm;
        fromComponent->remove();
        toComponent->remove();
        s = new SemaphoreArray(PATH, SEM_MUTEX, 1, owner);
        s->get();
        s->remove();
        s = new SemaphoreArray(PATH, SEM_SEMA, ROBOT_AMOUNT, owner);
        s->get();
        s->remove();
        s = new SemaphoreArray(PATH, SEM_SEMB, ROBOT_AMOUNT, owner);
        s->get();
        s->remove();
        s = new SemaphoreArray(PATH, SEM_SEME, ROBOT_AMOUNT, owner);
        s->get();
        s->remove();
        shm = new SharedMemory(PATH, SHM_PLAT, sizeof (struct plate), owner);
        shm->get();
        shm->remove();
    }
}

std::string iRobotB::msgToStr(enum iPlatMessage function)
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
