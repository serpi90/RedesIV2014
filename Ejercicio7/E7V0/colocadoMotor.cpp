#include "iColocadoMotor.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iColocadoMotor * i = new iColocadoMotor();
    Config conf("config.conf");
    unsigned time;
    while (true)
    {
        Helper::output(stdout, "ColocadoMotor: esperando el chasis secado.\n");
        i->esperarSecado();

        Helper::output(stdout, "ColocadoMotor: retirando el chasis iColocadoMotor.\n");
        time = Helper::doSleep(conf.getInt("retirar secado min", 1), conf.getInt("retirar secado max", 5));
        ss << "ColocadoMotor: tarde " << time << " segundos en retirar el chasis secado." << std::endl;
        Helper::output(stdout, ss);
        i->recibidoSecado();

        Helper::output(stdout, "ColocadoMotor: poniendo el motor.\n");

        time = Helper::doSleep(conf.getInt("colocar motor min", 1), conf.getInt("colocar motor max", 5));
        ss << "ColocadoMotor: tarde " << time << " segundos en poner el motor. espero para entregar." << std::endl;
        Helper::output(stdout, ss);

        i->entregarMotor();
        Helper::output(stdout, "ColocadoMotor: retiraron el chasis con motor, sigo trabajando\n");
    }
    return 0;
}
