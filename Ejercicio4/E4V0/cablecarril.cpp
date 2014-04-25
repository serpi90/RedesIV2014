#include "iCableCarril.h"
#include "Helper.h"
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
    while (true)
    {
        cc->estoy(abajo, personas);
        cc->cargarPersonas(abajo, personas);
        cc->estoy(arriba, personas);
        cc->cargarPersonas(arriba, personas);
    }
    return 0;
}

