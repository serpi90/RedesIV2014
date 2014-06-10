#include "Helper.h"
#include "Config.h"
#include "iProductor.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    std::string owner;
    srand(time(NULL));
    ss << "Productor: ";
    owner = ss.str();
    ss.str("");
    iProductor * i = new iProductor();
    Config conf("config.conf");
    int min, max, maxDiscos, maxProcesadores, maxMotherboards;
    struct orden o;
    min = conf.getInt("productor min", 1);
    max = conf.getInt("productor max", 5);
    maxDiscos = conf.getInt("max discos", 5);
    maxProcesadores = conf.getInt("max procesadores", 5);
    maxMotherboards = conf.getInt("max motherboards", 5);

    while (true)
    {
        o.discos = rand() % maxDiscos;
        o.procesadores = rand() % maxProcesadores;
        o.motherboards = rand() % maxMotherboards;
        Helper::doSleep(min, max);
        i->producirOrden(o);
        ss << owner << " produci " << o.discos << " discos, " << o.procesadores << " procesadores y " << o.motherboards << " motherboards." << std::endl;
        Helper::output(stdout, ss);
    }
    return 0;
}
