#include "includes.h"
#include "Helper.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sstream>

int main(int argc, char* argv[])
{
    std::stringstream ss;
    if (argc < 3)
    {
        ss << "Usage: " << argv[0] << " <H-Amount> <O-Amount>" << std::endl;
        Helper::output(stdout, ss, RED);
    }
    pid_t pid;
    unsigned h = atoi(argv[1]), o = atoi(argv[2]);
    for (unsigned i = 0; i < h; i++)
    {
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./hidrogeno", "hidrogeno", ss.str().c_str(), NULL);
            perror("hidrogeno - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("hidrogeno - fork: ");
        }
        ss.str("");
    }
    for (unsigned i = 0; i < o; i++)
    {
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./oxigeno", "oxigeno", ss.str().c_str(), NULL);
            perror("oxigeno - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("oxigeno - fork: ");
        }
        ss.str("");
    }
}