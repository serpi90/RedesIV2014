#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "iSampler.h"
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
    iSampler * s = new iSampler(number);
    Config conf("config.conf");
    struct sample sample;
    sample.id = number;

    while (true)
    {
        sample.id += SAMPLER_AMOUNT;
        s->placeSample(sample);
        ss << "Sampler placed sample: " << sample.id << std::endl;
        Helper::output(stdout, ss);
        ss << "Sampler simule trabajar durante " << Helper::doSleep(conf.getInt("sampler sleep min", 1), conf.getInt("sampler sleep max", 5)) << " segundos" << std::endl;
    }
    return 0;
}
