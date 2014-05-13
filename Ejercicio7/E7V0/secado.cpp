#include "iSecado.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iSecado * i = new iSecado();
    Config conf("config.conf");
    unsigned time;
    while (true)
    {
        Helper::output(stdout, "Secado: esperando el chasis pintado.\n");
        i->esperarPintado();

        Helper::output(stdout, "Secado: retirando el chasis pintado.\n");
        time = Helper::doSleep(conf.getInt("retirar pintado min", 1), conf.getInt("retirar pintado max", 5));
        ss << "Secado: tarde " << time << " segundos en retirar el chasis pintado." << std::endl;
        Helper::output(stdout, ss);
        i->recibidoPintado();

        Helper::output(stdout, "Secado: secando el chasis.\n");

        time = Helper::doSleep(conf.getInt("secar min", 1), conf.getInt("secar max", 5));
        ss << "Secado: tarde " << time << " segundos en secar. espero para entregar." << std::endl;
        Helper::output(stdout, ss);

        i->entregarSecado();
        Helper::output(stdout, "Secado: retiraron el chasis secado, sigo trabajando\n");
    }
    return 0;
}
