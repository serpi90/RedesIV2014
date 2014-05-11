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
        ss << "\033[1;36mSampler placed sample: \033[0m" << sample.id << std::endl;
        Helper::output(stdout, ss);
        ss << "Sampler fake worked during: " << Helper::doSleep(conf.getInt("sampler sleep min", 1), conf.getInt("sampler sleep max", 5)) << " segundos" << std::endl;
    }
    return 0;
}
