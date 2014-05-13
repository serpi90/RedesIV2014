#include "iPintado.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iPintado * i = new iPintado();
    Config conf("config.conf");
    unsigned time;
    while (true)
    {
        Helper::output(stdout, "Pintado: esperando el chasis.\n");
        i->esperarChasis();

        Helper::output(stdout, "Pintado: retirando el chasis.\n");
        time = Helper::doSleep(conf.getInt("retirar chasis min", 1), conf.getInt("retirar chasis max", 5));
        ss << "Pintado: tarde " << time << " segundos en retirar el chasis." << std::endl;
        Helper::output(stdout, ss);
        i->recibidoChasis();

        Helper::output(stdout, "Pintado: pintando el chasis.\n");

        time = Helper::doSleep(conf.getInt("pintar min", 1), conf.getInt("pintar max", 5));
        ss << "Pintado: tarde " << time << " segundos en pintar. espero para entregar." << std::endl;
        Helper::output(stdout, ss);

        i->entregarPintado();
        Helper::output(stdout, "Pintado: retiraron el chasis pintado, sigo trabajando\n");
    }
    return 0;
}
