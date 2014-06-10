#include "Helper.h"
#include "Config.h"
#include "iEncargado.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    std::string owner;
    srand(time(NULL));
    ss << "Encargado motherboards: ";
    owner = ss.str();
    ss.str("");
    iEncargado * i = new iEncargado(MOTHERBOARDS);
    Config conf("config.conf");
    int min, max;
    struct orden o;
    min = conf.getInt("encargado motherboards min", 1);
    max = conf.getInt("encargado motherboards max", 5);

    while (true)
    {
        Helper::doSleep(min, max);
        o = i->consumirOrden();
        ss << owner << " consumi orden con " << o.motherboards << " motherboards." << std::endl;
        Helper::output(stdout, ss);
    }
    return 0;
}
