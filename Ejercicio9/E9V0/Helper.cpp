#include <sstream>
#include <cstdio>
#include "Helper.h"
#include <unistd.h>
void Helper::output(FILE* file, std::stringstream &ss, std::string colour)
{
    output(file, ss.str(), colour);
    ss.str("");
}

void Helper::output(FILE* file, std::string s, std::string colour)
{
    s = colour + s 
+ NORMAL;
    if (write(fileno(file), s.c_str(), s.size()))
    {
    }
}

unsigned int Helper::doSleep(int min, int max)
{
    unsigned int time = min + rand() % (max - min + 1);
    sleep(min + rand() % (max - min + 1));
    return time;
}

std::string Helper::msgToString(enum iMessageType m)
{
    switch (m)
    {
        case ACTIVAR:
            return std::string(BROWN) + "ACTIVAR" + NORMAL;
            break;
        case ACTIVADO:
            return std::string(BROWN) + "ACTIVADO" + NORMAL;
            break;
        case TERMINAR:
            return std::string(BROWN) + "TERMINAR" + NORMAL;
            break;
        case TERMINADO:
            return std::string(BROWN) + "TERMINADO" + NORMAL;
            break;
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}

std::string Helper::msgToString(enum kind kind)
{
    switch (kind)
    {
        case OXIGENO:
            return std::string(CYAN) + "OXIGENO" + NORMAL;
            break;
        case HIDROGENO:
            return std::string(CYAN) + "HIDROGENO" + NORMAL;
            break;
        default:
            std::stringstream ss;
            ss << kind;
            return ss.str();
    }
}
