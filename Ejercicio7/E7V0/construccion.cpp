#include "iConstruccion.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iConstruccion * i = new iConstruccion();
    Config conf("config.conf");
    while (true)
    {
        ss << "Construccion: tarde " << Helper::doSleep(conf.getInt("construccion min", 1), conf.getInt("construccion max", 5)) << " segundos en construir el chasis. espero para entregar." << std::endl;
        Helper::output(stdout, ss);
        i->entregarChasis();
        Helper::output(stdout, "Construccion: retiraron el chasis, sigo trabajando\n");
    }
    return 0;
}
