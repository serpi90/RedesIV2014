#include "Helper.h"
#include "Config.h"
#include "iEncargado.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    std::string owner;
    srand(time(NULL));
    ss << "Encargado procesadores: ";
    owner = ss.str();
    ss.str("");
    iEncargado * i = new iEncargado(PROCCESSORS);
    Config conf("config.conf");
    int min, max;
    struct orden o;
    min = conf.getInt("encargado procesadores min", 1);
    max = conf.getInt("encargado procesadores max", 5);

    while (true)
    {
        Helper::doSleep(min, max);
        o = i->consumirOrden();
        ss << owner << " consumi orden con " << o.procesadores << " procesadores." << std::endl;
        Helper::output(stdout, ss);
    }
    return 0;
}
