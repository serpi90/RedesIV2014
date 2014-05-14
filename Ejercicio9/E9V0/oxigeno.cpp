#include "Helper.h"
#include "Config.h"
#include "iOxigeno.h"
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
    ss << "Oxigeno " << number << ": ";
    owner = ss.str();
    ss.str("");
    iOxigeno * i = new iOxigeno(number);
    Config conf("config.conf");
    unsigned tiempo;
    int min, max;
    min = conf.getInt("oxigeno min", 1);
    max = conf.getInt("oxigeno max", 5);
    
    i->activar();
    tiempo = Helper::doSleep(min, max);
    ss << owner << " trabaje durante " << tiempo << " segundos" << std::endl;
    Helper::output(stdout, ss);
    i->terminar();
    ss << owner << " termine" << std::endl;
    Helper::output(stdout, ss);

    return 0;
}
