#include "includes.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int main()
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        execlp("./construccion", "construccion", NULL);
        perror("construccion - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("construccion - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./pintado", "pintado", NULL);
        perror("pintado - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("pintado - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./secado", "secado", NULL);
        perror("secado - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("secado - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./colocadoMotor", "colocadoMotor", NULL);
        perror("colocadoMotor - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("colocadoMotor - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./colocadoInterior", "colocadoInterior", NULL);
        perror("colocadoInterior - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("colocadoInterior - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./colocadoExterior", "colocadoExterior", NULL);
        perror("colocadoExterior - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("colocadoExterior - fork: ");
    }

}