#include "iCableCarril.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 1)
    {
        cerr << "Usage: " << argv[0] << " <number>" << endl;
        exit(EXIT_FAILURE);
    }
    long number = atoi(argv[1]);
    iCableCarril * cc = new iCableCarril(number);
    stringstream ss;
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
        cc->estoy(ABAJO, personas);
        // Aviso que terminaron de bajar todos para que comiencen a subir.
        cc->cargarPersonas(ABAJO, personas);

        ss << "CableCarril viajando " << endl;
        Helper::output(stdout, ss);
        ss << "CableCarril viaje durante " << Helper::doSleep(conf.getInt("viaje min", 0), conf.getInt("viaje max", 0)) << " segundos" << endl;
        Helper::output(stdout, ss);

        // Aviso que llegue arriba.
        cc->estoy(ARRIBA, personas);
        // Aviso que terminaron de bajar todos para que comiencen a subir.
        cc->cargarPersonas(ARRIBA, personas);
        Helper::doSleep(conf.getInt("viaje min", 0), conf.getInt("viaje max", 0));
    }
    return 0;
}

