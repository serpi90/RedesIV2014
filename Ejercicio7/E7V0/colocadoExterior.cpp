#include "iColocadoExteriores.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iColocadoExteriores * i = new iColocadoExteriores();
    Config conf("config.conf");
    unsigned tiempo;
    unsigned puertas = 0, capos = 0, baules = 0;
    while (true)
    {
        Helper::output(stdout, "ColocadoExteriores: esperando el chasis con interiores.\n");
        i->esperarInteriores();

        if (puertas < 2)
        {
            Helper::output(stdout,"Esperando por mas puertas\n");
            puertas += i->reponerPuertas();
        }
        if (!capos)
        {
            Helper::output(stdout,"Esperando por mas capos\n");
            capos = i->reponerCapos();
        }
        if (!baules)
        {
            Helper::output(stdout,"Esperando por mas baules\n");
            i->reponerBaules();
        }
        Helper::output(stdout, "ColocadoExteriores: retirando el chasis con motor.\n");
        tiempo = Helper::doSleep(conf.getInt("retirar con interiores min", 1), conf.getInt("retirar con interiores max", 5));
        ss << "ColocadoExteriores: tarde " << tiempo << " segundos en retirar el chasis con interiores." << std::endl;
        Helper::output(stdout, ss);
        i->recibidoInteriores();
        puertas -= 2;
        capos--;
        baules--;

        Helper::output(stdout, "ColocadoExteriores: colocando exteriores.\n");

        tiempo = Helper::doSleep(conf.getInt("colocar exterior min", 1), conf.getInt("colocar exterior max", 5));
        ss << "ColocadoExteriores: tarde " << tiempo << " segundos en colocar exteriores." << std::endl;
        Helper::output(stdout, ss, PURPLE);
    }
    return 0;
}
