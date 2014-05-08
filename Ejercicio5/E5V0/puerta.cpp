#include <sstream>
#include <unistd.h>
#include "iPuerta.h"
#include "Helper.h"
#include "Config.h"

int main(int argc, char** argv)
{
    std::stringstream ss;
    std::string arg;
    long number;
    if (argc > 2)
    {
        arg = std::string(argv[1]);
        number = atoi(argv[2]);
    }
    if (arg != "-abajo" && arg != "-arriba")
    {
        ss << "Usage: " << argv[0] << "<-abajo | -arriba> <numero>" << std::endl;
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    iPuerta * s = new iPuerta(arg == "-abajo" ? ABAJO : ARRIBA, number);

    Config conf("puerta.conf");

    while (true)
    {
        s->esperarPersona();
        // TODO esperarPersona podria devolvoer el id de la persona que entro
    }
    return 0;
}

