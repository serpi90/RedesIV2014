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
        case TERMINE:
            return std::string(BROWN) + "TERMINE" + NORMAL;
            break;
        case CONTINUAR:
            return std::string(BROWN) + "CONTINUAR" + NORMAL;
            break;
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}

std::string Helper::msgToString(long l)
{
    std::stringstream ss;
    if (l >= M_EMBOTELLADORA && l < M_EMBOTELLADORA + CANTIDAD_EMBOTELLADORAS)
    {
        ss << BLUE << "Embotelladora " << (l - M_EMBOTELLADORA) << NORMAL;
    } else
    {
        ss << BG_RED << WHITE << l << NORMAL;
    }
    return ss.str();
}
