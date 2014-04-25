/* 
 * File:   RobotB.cpp
 * Author: Julian Maestri
 *
 * Created on April 6, 2014, 1:30 PM
 */

#include <cstdlib>
#include <string>
#include "includes.h"
#include "iRobotB.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    std::stringstream ss;
    if (argc < 1)
    {
        ss << "Usage " << argv[0] << " id" << std::endl;
        write(fileno(stderr),ss.str().c_str(),ss.str().size());
        exit(EXIT_FAILURE);
    }
    int id = atoi(argv[1]);
    ss << "Robot B" << id;
    std::string owner = ss.str();

    iRobotB * plate = new iRobotB(id, owner);
    ss << ": ";
    owner = ss.str();

    while (plate->hayDispositivosORobots())
    {
        ss.str("");
        ss << owner << "por Sacar" << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        ss.str("");
	if (plate->sacar())
        {
            ss << owner << "saque exitosamente." << std::endl;
        } else
        {
            ss << owner << "no hay dispositivos para sacar." << std::endl;
        }
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        sleep(rand() % 1 + 1);
    }
    delete plate;
    return 0;
}

