#include "iColocadoExteriores.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iColocadoExteriores * i = new iColocadoExteriores();
    Config conf("config.conf");
    unsigned time;
    while (true)
    {
        Helper::output(stdout, "ColocadoExteriores: esperando el chasis con interiores.\n");
        i->esperarInteriores();

        Helper::output(stdout, "ColocadoExteriores: retirando el chasis con motor.\n");
        time = Helper::doSleep(conf.getInt("retirar con interiores min", 1), conf.getInt("retirar con interiores max", 5));
        ss << "ColocadoExteriores: tarde " << time << " segundos en retirar el chasis con interiores." << std::endl;
        Helper::output(stdout, ss);
        i->recibidoInteriores();

        Helper::output(stdout, "ColocadoExteriores: colocando exteriores.\n");

        time = Helper::doSleep(conf.getInt("colocar exterior min", 1), conf.getInt("colocar exterior max", 5));
        ss << "ColocadoExteriores: tarde " << time << " segundos en colocar exteriores." << std::endl;
        Helper::output(stdout, ss, PURPLE);
    }
    return 0;
}
