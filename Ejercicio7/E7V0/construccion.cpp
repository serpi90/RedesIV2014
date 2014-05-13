#include "iConstruccion.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    std::stringstream ss;
    iConstruccion * i = new iConstruccion();
    Config conf("config.conf");
    unsigned materiales = 0;
    while (true)
    {
        if (!materiales)
        {
            Helper::output(stdout, "Esperando por mas materiales\n");
            materiales = i->reponer();
        }
        ss << "Construccion: tarde " << Helper::doSleep(conf.getInt("construccion min", 1), conf.getInt("construccion max", 5)) << " segundos en construir el chasis. espero para entregar." << std::endl;
        Helper::output(stdout, ss);
        materiales--;
        
        i->entregarChasis();
        Helper::output(stdout, "Construccion: retiraron el chasis, sigo trabajando\n");
    }
    return 0;
}
