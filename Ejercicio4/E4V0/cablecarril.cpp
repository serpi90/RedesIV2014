#include "iCableCarril.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>

int main()
{
    iCableCarril * cc = new iCableCarril();
    bool abajo = true, arriba = false;
    std::stringstream ss;
    long personas[CC_SIZE];
    int cp;
    for (cp = 0; cp < CC_SIZE; cp++)
    {
        personas[cp] = 0;
    }
    Config conf("cablecarril.conf");
    while (true)
    {
        // Aviso que llegue abajo.
        cc->estoy(abajo, personas);
        // Aviso que terminaron de bajar todos para que comiencen a subir.
        cc->cargarPersonas(abajo, personas);

        ss << "CableCarril viaje durante " << Helper::doSleep(conf.getInt("viaje min", 0), conf.getInt("viaje max", 0)) << " segundos" << std::endl;
        Helper::output(stdout, ss);

        // Aviso que llegue arriba.
        cc->estoy(arriba, personas);
        // Aviso que terminaron de bajar todos para que comiencen a subir.
        cc->cargarPersonas(arriba, personas);
        Helper::doSleep(conf.getInt("viaje min", 0), conf.getInt("viaje max", 0));
    }
    return 0;
}

