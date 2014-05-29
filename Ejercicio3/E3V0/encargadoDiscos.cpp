#include "Helper.h"
#include "Config.h"
#include "iEncargado.h"
#include <sstream>

int main(int argc, char* argv[])
{
    std::stringstream ss;
    std::string owner;
    long number = (long) DISCOS;
    srand(time(NULL) + number);
    ss << "Encargado discos " << number << ": ";
    owner = ss.str();
    ss.str("");
    iEncargado * i = new iEncargado(number, DISCOS);
    Config conf("config.conf");
    int min, max;
    struct orden o;
    min = conf.getInt("encargado discos min", 1);
    max = conf.getInt("encargado discos max", 5);

    while (true)
    {
        Helper::doSleep(min, max);
        o = i->consumirOrden();
        ss << owner << " consumi orden con " << o.discos << " discos." << std::endl;
        Helper::output(stdout, ss);
    }
    return 0;
}