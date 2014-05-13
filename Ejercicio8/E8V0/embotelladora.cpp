#include "iEmbotelladora.h"
#include "Helper.h"
#include "Config.h"
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
    ss << "Embotelladora " << number << ": ";
    owner = ss.str();
    ss.str("");
    iEmbotelladora * i = new iEmbotelladora(number);
    Config conf("config.conf");
    unsigned tiempo;
    int min, max;
    min = conf.getInt("embotellado min", 1);
    max = conf.getInt("embotellado max", 5);
    while (true)
    {
        ss << owner << "trabajando" << std::endl;
        Helper::output(stdout, ss, YELLOW);
        tiempo = Helper::doSleep(min, max);
        ss << owner << "trabaje durante " << tiempo << " espero a las demas" << std::endl;
        Helper::output(stdout, ss, GREEN);
        i->termine();
    }
    return 0;
}
