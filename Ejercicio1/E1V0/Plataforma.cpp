/* 
 * File:   Plataforma.cpp
 * Author: lubuntu
 * 
 * Created on April 6, 2014, 2:20 PM
 */

#include "Plataforma.h"

Plataforma::Plataforma(int number, char rtype)
{
    std::stringstream ss;
    this->id = (rtype == 'a' ? M_PLATA : M_PLATB) + number;
    ss << "\033[33m" << "Plataforma " << (char) (rtype^32) << number << "\033[0m";
    owner = ss.str();
    toInterface = new Queue(PATH, Q_PLAT_TO_INTERFACE, owner);
    fromInterface = new Queue(PATH, Q_PLAT_FROM_INTERFACE, owner);
    toInterface->get();
    fromInterface->get();
    mutex = new Semaphore(PATH, SEM_MUTEX, owner);
    semsA = new SemaphoreArray(PATH, SEM_SEMA, ROBOT_AMOUNT, owner);
    semsB = new SemaphoreArray(PATH, SEM_SEMB, ROBOT_AMOUNT, owner);
    semsE = new SemaphoreArray(PATH, SEM_SEME, ROBOT_AMOUNT, owner);
    shm = new SharedMemory(PATH, SHM_PLAT, sizeof (struct plate), owner);
    mutex->get();
    semsA->get();
    semsB->get();
    semsE->get();
    shm->get();
    plate = (struct plate*) shm->attach();
}

void Plataforma::init()
{
    std::stringstream ss;
    iMessage buf;
    while (true)
    {
        // Activar ACT_PER_REQUEST dispositivos al azar
        for (int j = 0; j < ACT_PER_REQUEST; j++)
        {
            mutex->wait();
            plate->slots[rand() % PLAT_SIZE].dispositive.status = ACTIVE;
            mutex->post();
        }
        fromInterface->receive((void*) &buf, sizeof (buf), id);
	ss << owner << ": Recibi " << msgToStr(buf.function) << " de Robot " << (buf.robot) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        ss.str("");
        switch (buf.function)
        {
            case RESERVE:
                buf.function = RESERVE_OK;
                buf.content.location = reserve(buf.robot);
                break;
            case PUT:
                buf.function = PUT_OK;
                put(buf.robot, buf.content.putMessage.location, buf.content.putMessage.idDispositive);
                break;
            case TAKE:
                buf.function = take(buf.robot) ? TAKE_OK : TAKE_NONE;
                break;
            case SHUTDOWN:
                buf.function = shutdown(buf.robot, buf.content.rtype) ? SHUTDOWN_LAST : SHUTDOWN_OK;
                break;
            case QUERY_ROBOTS_OR_DISPOSITIVES:
                buf.function = haveDispOrRobots() ? QUERY_ROBOTS_OR_DISPOSITIVES_YES : QUERY_ROBOTS_OR_DISPOSITIVES_NO;
                break;
            default:
		ss << owner << ": Recibi desconocido" << msgToStr(buf.function) << " de Robot " << (buf.robot) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
                write(fileno(stderr),ss.str().c_str(),ss.str().size());
                ss.str("");
                break;
        }
        buf.type = buf.senderType;
        buf.senderType = id;
        toInterface->send((void*) &buf, sizeof (buf));
	ss << owner << ": Envie Respuesta " << msgToStr(buf.function) << " a Robot " << (buf.robot) << " mq=" << buf.type << " sq=" << buf.senderType << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        ss.str("");
    }
}

int Plataforma::reserve(int robot)
{
    std::stringstream ss;
    mutex->wait();
    // Esperar a que termine B
    if (plate->statusB[robot] == WORKING)
    {
        plate->statusA[robot] = IDLE;
        mutex->post();
	ss << owner << ": \033[1;36mInactivo\033[0m hasta a que termine B" << robot << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        ss.str("");
        semsA->wait(robot);
        mutex->wait();
    }
    // Mientras este llena
    while (plate->storedAmount == PLAT_SIZE)
    {
        ss << owner << ": \033[1;36mEsperando\033[0m a que haya lugar" << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
	ss.str("");
        plate->statusA[robot] = IDLE;
        plate->statusE[robot] = WAITING;
        mutex->post();
        semsE->wait(robot);
        mutex->wait();
        plate->statusE[robot] = NOT_WAITING;
    }
    plate->statusA[robot] = WORKING;
    ss << owner << ": \033[1;36mTrabajando\033[0m A" << robot << std::endl;
    write(fileno(stdout),ss.str().c_str(),ss.str().size());
    ss.str("");
    // Reservar
    int i;
    for (i = 0; i < PLAT_SIZE && plate->slots[i].state != EMPTY; i++);
    plate->slots[i].state = RESERVED;
    plate->storedAmount++;
    ss << owner << ": reserve lugar " << i << std::endl;
    write(fileno(stdout),ss.str().c_str(),ss.str().size());
    ss.str("");
    mutex->post();
    return i;
}

void Plataforma::put(int robot, int location, long id)
{
    std::stringstream ss;
    mutex->wait();
    plate->slots[location].state = OCCUPIED;
    plate->slots[location].dispositive.id = id;
    plate->slots[location].dispositive.status = INACTIVE;
    plate->statusA[robot] = IDLE;
    ss << owner << ": puse en el lugar " << location << std::endl;
    write(fileno(stdout),ss.str().c_str(),ss.str().size());
    ss.str("");
    if (plate->statusB[robot] == IDLE)
    {
        plate->statusB[robot] = WORKING;
        semsB->post(robot);
	ss << owner << ": \033[1;36mActivando\033[0m a B" << robot << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        ss.str("");
    }
    mutex->post();
}

bool Plataforma::take(int robot)
{
    std::stringstream ss;
    mutex->wait();
    // Espero a que termine A
    if (plate->statusA[robot] == WORKING)
    {
        plate->statusB[robot] = IDLE;
        mutex->post();
	ss << owner << ": \033[1;36mInactivo\033[0m hasta a que termine A" << robot << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        ss.str("");
        semsB->wait(robot);
        mutex->wait();
    }
    plate->statusB[robot] = WORKING;
    ss << owner << ": \033[1;36mTrabajando\033[0m B" << robot << std::endl;
    write(fileno(stdout),ss.str().c_str(),ss.str().size());
    ss.str("");
    int i = PLAT_SIZE;
    // Busco un dispositivo para sacar
    if (plate->storedAmount)
    {
        for (i = 0; i < PLAT_SIZE &&
                plate->slots[i].state != OCCUPIED &&
                plate->slots[i].dispositive.status != ACTIVE; i++);
    }
    // Si esta vacio
    bool response = true;
    if (i == PLAT_SIZE)
    {
        ss << owner << ": no hay dispositivos para sacar" << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
	ss.str("");
        response = false;
    } else
    {
        plate->slots[i].state = EMPTY;
        plate->storedAmount--;
    }
    plate->statusB[robot] = IDLE;

    // Si estaba llena destrabo a los robots A (el mutex lo mantengo).
    if (plate->storedAmount == PLAT_SIZE - 1)
    {
        for (int i = 0; i < ROBOT_AMOUNT; i++)
        {
            if (plate->statusE[i] == WAITING)
            {
                plate->statusE[i] = NOT_WAITING;
		ss << owner << ": \033[1;36mDestrabando\033[0m A" << i << std::endl;
                write(fileno(stdout),ss.str().c_str(),ss.str().size());
                ss.str("");
                semsE->post(i);
            }
        }
    }
    if (plate->statusA[robot] == IDLE && plate->statusE[robot] == NOT_WAITING)
    {
        plate->statusA[robot] = WORKING;
        semsA->post(robot);
    }
    mutex->post();
    return response;
}

bool Plataforma::shutdown(int robot, char rtype)
{
    std::stringstream ss;
    bool response = true;
    mutex->wait();
    if (rtype == 'a')
    {
        plate->activeRobotsA--;
        plate->statusA[robot] = OFF;
	if( plate->statusB[robot] == IDLE ) {
            semsB->post(robot);
	}
    } else
    {
        plate->activeRobotsB--;
        plate->statusB[robot] = OFF;
    }
    response = plate->activeRobotsB == 0;
    ss << owner << ": Desactive a " << (char) (rtype|32) << robot << std::endl;
    ss << owner << ": RobotsA: " << plate->activeRobotsA << " RobotsB: " << plate->activeRobotsB << std::endl;
    write(fileno(stdout),ss.str().c_str(),ss.str().size());
    ss.str("");
    mutex->post();
    return response;
}

bool Plataforma::haveDispOrRobots()
{
    bool result;
    mutex->wait();
    result = plate->activeRobotsA || plate->storedAmount;
    mutex->post();
    return result;
}

std::string Plataforma::msgToStr(enum iPlatMessage function)
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
