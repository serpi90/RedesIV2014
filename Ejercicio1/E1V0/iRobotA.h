/* 
 * File:   iPlataforma.h
 * Author: Julian Maestri
 *
 * Created on April 6, 2014, 1:34 PM
 */

#ifndef IROBOTA_H
#define	IROBOTA_H

#include "includes.h"
#include "Queue.h"
#include "Semaphore.h"
#include "SharedMemory.h"
#include <string>
#include <sstream>

class iRobotA {
public:
    iRobotA(int id, std::string owner, bool first = false);
    ~iRobotA();
    int reservar(void);
    void poner(int location);
    bool avisarApagado();
private:
    std::string msgToStr(enum iPlatMessage function);
    std::string owner;
    int number;
    Queue * fromComponent;
    Queue * toComponent;
};

#endif	/* IROBOTA_H */

