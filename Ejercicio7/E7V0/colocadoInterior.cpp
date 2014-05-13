#include "iColocadoInteriores.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iColocadoInteriores * i = new iColocadoInteriores();
    Config conf("config.conf");
    unsigned time;
    while (true)
    {
        Helper::output(stdout, "ColocadoInteriores: esperando el chasis con motor.\n");
        i->esperarMotor();

        Helper::output(stdout, "ColocadoInteriores: retirando el chasis con motor.\n");
        time = Helper::doSleep(conf.getInt("retirar con motor min", 1), conf.getInt("retirar con motor max", 5));
        ss << "ColocadoInteriores: tarde " << time << " segundos en retirar el chasis con motor." << std::endl;
        Helper::output(stdout, ss);
        i->recibidoMotor();

        Helper::output(stdout, "ColocadoInteriores: colocando interiores.\n");

        time = Helper::doSleep(conf.getInt("colocar interior min", 1), conf.getInt("colocar interior max", 5));
        ss << "ColocadoInteriores: tarde " << time << " segundos en colocar interiores. espero para entregar." << std::endl;
        Helper::output(stdout, ss);

        i->entregarInteriores();
        Helper::output(stdout, "ColocadoInteriores: retiraron el chasis con interiores, sigo trabajando\n");
    }
    return 0;
}
