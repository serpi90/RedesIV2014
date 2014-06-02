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
    s = colour + s + NORMAL;
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
        case QUIERO_LEER:
            return std::string(BROWN) + "QUIERO_LEER" + NORMAL;
            break;
        case LEE:
            return std::string(BROWN) + "LEE" + NORMAL;
            break;
        case QUIERO_ESCRIBIR:
            return std::string(BROWN) + "QUIERO_ESCRIBIR" + NORMAL;
            break;
        case ESCRIBI:
            return std::string(BROWN) + "ESCRIBI" + NORMAL;
            break;
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}
