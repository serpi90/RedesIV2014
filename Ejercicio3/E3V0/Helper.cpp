#include <sstream>
#include <cstdio>
#include "Helper.h"

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
        case PRODUCIR_ORDEN:
            return std::string(BROWN) + "PRODUCIR ORDEN" + NORMAL;
            break;
        case CONSUMIR_ORDEN:
            return std::string(BROWN) + "CONSUMIR ORDEN" + NORMAL;
            break;
        case CONSUMISTE_ORDEN:
            return std::string(BROWN) + "CONSUMISTE ORDEN" + NORMAL;
            break;
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}
