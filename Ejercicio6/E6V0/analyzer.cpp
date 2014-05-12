#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "iAnalyzer.h"
#include "Helper.h"
#include "Config.h"

int main(int argc, char* argv[])
{
    std::stringstream ss;
    if (argc < 2)
    {
        ss << "Usage: " << argv[0] << " <number>" << std::endl;
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    long number = (long) atoi(argv[1]);
    iAnalyzer * a = new iAnalyzer(number);
    Config conf("config.conf");
    struct sample sample;
    sample.id = number;

    while (true)
    {
        sample = a->getSample();
        ss << "\033[1;37mAnalyzer got sample: \033[0m" << sample.id << std::endl;
        Helper::output(stdout, ss);
        ss << "Analyzer fake worked during: " << Helper::doSleep(conf.getInt("analyzer sleep min", 1), conf.getInt("analyzer sleep max", 5)) << " seconds" << std::endl;
    }
    return 0;
}
