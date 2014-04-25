/* 
 * File:   iRobotB.h
 * Author: Julian Maestri
 *
 * Created on April 6, 2014, 1:34 PM
 */

#ifndef IPLATAFORMA_H
#define	IPLATAFORMA_H

#include "includes.h"
#include "Queue.h"
#include "Semaphore.h"
#include "SharedMemory.h"
#include <string>
#include <sstream>

class iRobotB {
public:
    iRobotB(int id, std::string owner);
    ~iRobotB();
    bool sacar(void);
    bool hayDispositivosORobots(void);
    bool avisarApagado();
private:
    std::string msgToStr(enum iPlatMessage function);
    std::string owner;
    int number;
    Queue * fromComponent;
    Queue * toComponent;
};

#endif	/* IPLATAFORMA_H */

