#include <sstream>
#include <unistd.h>
#include "iSala.h"
#include "Helper.h"
#include "Config.h"

int main(int argc, char** argv)
{
    std::stringstream ss;
    std::string arg;
    if (argc > 1)
    {
        arg = std::string(argv[1]);
    }
    if (arg != "-abajo" && arg != "-arriba")
    {
        ss << "Usage: " << argv[0] << "<-abajo | -arriba>" << std::endl;
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    iSala * s = new iSala(arg == "-abajo");

    Config conf( "sala.conf" );

    while (true)
    {
        s->esperarPersona();
        // TODO que esperar persona devuelva el id de la persona que entro
    }
    return 0;
}

