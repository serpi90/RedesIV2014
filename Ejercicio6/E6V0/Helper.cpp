#include <sstream>
#include <cstdio>
#include "Helper.h"

void Helper::output(FILE* file, std::stringstream &ss)
{
    output(file, ss.str());
    ss.str("");
}

void Helper::output(FILE* file, std::string s)
{
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
        case PLACE_SAMPLE:
            return "\033[1;34miPLACE SAMPLE\033[0m";
        case SAMPLE_PLACED:
            return "\033[34miSAMPLE PLACED\033[0m";
        case GET_SAMPLE:
            return "\033[1;32miGET SAMPLE\033[0m";
        case GOT_SAMPLE:
            return "\033[32miGOT SAMPLE\033[0m";
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}

std::string Helper::msgToString(enum workingStatus s)
{
    switch (s)
    {
        case WAITING: return "\033[31mWAITING\033[0m";
        case WORKING: return "\033[32mWORKING\033[0m";
        default:
            std::stringstream ss;
            ss << s;
            return ss.str();
    }
}

std::string Helper::msgToString(enum analyisisStatus s)
{
    switch (s)
    {
        case NOT_ANALYZED: return "\033[31mNOT ANALYZED\033[0m";
        case ANALYZED: return "\033[31mANALYZED\033[0m";
        default:
            std::stringstream ss;
            ss << s;
            return ss.str();
    }
}