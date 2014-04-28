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
    for (int i = 0; i < CC_SIZE; i++)
    {
        personas[i] = 0;
    }
    Config conf("cablecarril.conf");
    while (true)
    {
	// Aviso que llegue abajo.
        cc->estoy(abajo, personas);
	// Aviso que terminaron de bajar todos para que comiencen a subir.
        cc->cargarPersonas(abajo, personas);
	Helper::doSleep( conf.getInt("viaje min") , conf.getInt("viaje max") );
	Helper::output( stdout, "CableCarril Viajando" );
	// Aviso que llegue arriba.
        cc->estoy(arriba, personas);
	// Aviso que terminaron de bajar todos para que comiencen a subir.
        cc->cargarPersonas(arriba, personas);
	Helper::doSleep( conf.getInt("viaje min") , conf.getInt("viaje max") );
    }
    return 0;
}

