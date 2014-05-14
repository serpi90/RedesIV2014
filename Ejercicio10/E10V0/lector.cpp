#include "Helper.h"
#include "Config.h"
#include "iLector.h"
#include <sstream>

int main(int argc, char* argv[])
{
    std::stringstream ss;
    std::string owner;
    if (argc < 2)
    {
        ss << "Usage: " << argv[0] << " <number>" << std::endl;
        Helper::output(stderr, ss, RED);
        exit(EXIT_FAILURE);
    }
    long number = atoi(argv[1]);
    srand(time(NULL) + number);
    ss << "Lector " << number << ": ";
    owner = ss.str();
    ss.str("");
    iLector * i = new iLector(number);
    Config conf("config.conf");

    Helper::output(stdout, owner + " por leer\n");
    double cotizacion = i->leer();

    ss << owner << " lei " << cotizacion << std::endl;
    Helper::output(stdout, ss);

    return 0;
}
