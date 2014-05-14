#include "Helper.h"
#include "Config.h"
#include "iEscritor.h"
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
    ss << "Escritor " << number << ": ";
    owner = ss.str();
    ss.str("");
    iEscritor * i = new iEscritor(number);
    Config conf("config.conf");
    int min, max;
    min = conf.getInt("cotizacion min", 1);
    max = conf.getInt("cotizacion max", 5);

    double cotizacion = min + rand() % (max - min) + rand() % 100 / 100.0;
    ss << owner << " por escribir " << cotizacion << std::endl;
    Helper::output(stdout, ss);

    i->escribir(cotizacion);

    ss << owner << " escribi " << cotizacion << std::endl;
    Helper::output(stdout, ss);

    return 0;
}
