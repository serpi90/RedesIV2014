/* 
 * File:   RobotA.cpp
 * Author: Julian Maestri
 *
 * Created on April 6, 2014, 1:30 PM
 */

#include <cstdlib>
#include <string>
#include "includes.h"
#include "iRobotA.h"

int main(int argc, char** argv)
{
    std::stringstream ss;
    if (argc < 4)
    {
        ss << "Usage " << argv[0] << " <id | --first | -f> <piles> <dispositives> " << std::endl;
        write(fileno(stderr),ss.str().c_str(),ss.str().size());
        exit(EXIT_FAILURE);
    }
    pid_t child;
    int id;
    int piles = atoi(argv[2]);
    int dispositives = atoi(argv[3]);
    std::string owner;
    iRobotA * plate;
    if ( std::string(argv[1]) == "--first" || std::string(argv[1]) == "-f" )
    {
        owner = "Robot A0";
        id = 0;
        plate = new iRobotA(id, owner, true);
        std::string sid, p, d;
	ss << piles;
        p = ss.str();
        ss.str("");
        ss << dispositives;
        d = ss.str();
        ss.str("");
        for (int i = 1; i < 4; i++)
        {
            ss << i;
  	    sid = ss.str();
            ss.str("");
            child = fork();
            if (child == 0)
            {
                execlp("./RobotA", "RobotA", sid.c_str(), p.c_str(), d.c_str(), NULL);
                perror("RobotA - execlp: ");
                exit(EXIT_FAILURE);
            } else if (child < 0)
            {
                perror("RobotA - fork: ");
            }
        }
    } else
    {
        id = atoi(argv[1]);
        ss << "Robot A" << id;
        owner = ss.str();
        ss.str("");
	plate = new iRobotA(id, owner);
    }
    ss.str("");
    ss << id;
    child = fork();
    if (child == 0)
    {
        execlp("./RobotB", "RobotB", ss.str().c_str(), NULL);
        perror("RobotB - execlp: ");
        exit(EXIT_FAILURE);
    } else if (child < 0)
    {
        perror("RobotB - fork: ");
    }
    ss.str("");
    // Program itself
    while (piles > 1 && dispositives)
    {
        piles -= 2;
        dispositives--;
        ss.str("");
	ss << owner << ": por Reservar" << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        int location = plate->reservar();
        ss.str("");
	ss << owner << ": reserva exitosa" << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        sleep(rand() % 1 + 1);
	ss.str("");
	ss << owner << ": por Poner" << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        plate->poner(location);
        ss.str("");
	ss << owner << ": puse dispositivo en la bandeja" << std::endl;
        write(fileno(stdout),ss.str().c_str(),ss.str().size());
        sleep(rand() % 1 + 1);
    }
    delete plate;
    return 0;
}

