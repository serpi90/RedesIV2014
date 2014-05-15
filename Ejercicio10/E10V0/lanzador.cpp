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
        ss << "Usage: " << argv[0] << " <R-Amount> <W-Amount>" << std::endl;
        Helper::output(stdout, ss, RED);
    }
    pid_t pid;
    unsigned r = atoi(argv[1]), w = atoi(argv[2]);

    for (unsigned i = 0; i < w; i++)
    {
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./escritor", "escritor", ss.str().c_str(), NULL);
            perror("escritor - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("escritor - fork: ");
        }
        ss.str("");
    }

    for (unsigned i = 0; i < r; i++)
    {
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./lector", "lector", ss.str().c_str(), NULL);
            perror("lector - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("lector - fork: ");
        }
        ss.str("");
    }
}