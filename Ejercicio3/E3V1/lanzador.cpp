#include "includes.h"
#include "Helper.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sstream>

int main(int argc, char* argv[])
{
    std::stringstream ss;
    unsigned prod;
    if (argc < 2)
    {
        prod = CANT_PRODUCTORES;
    } else
    {
        prod = atoi(argv[1]);
    }
    pid_t pid;

    for (unsigned i = 0; i < prod; i++)
    {
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./productor", "productor", ss.str().c_str(), NULL);
            perror("productor - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("productor - fork: ");
        }
        ss.str("");
    }


    pid = fork();
    if (pid == 0)
    {
        execlp("./encargadoDiscos", "encargadoDiscos", NULL);
        perror("encargadoDiscos - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("encargadoDiscos - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./encargadoProcesadores", "encargadoProcesadores", NULL);
        perror("encargadoProcesadores - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("encargadoProcesadores - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./encargadoMotherboards", "encargadoMotherboards", NULL);
        perror("encargadoMotherboards - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("encargadoMotherboards - fork: ");
    }
}