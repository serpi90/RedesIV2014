#include "includes.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sstream>

int main()
{
    std::stringstream ss;
    pid_t pid;
    for (unsigned i = 0; i < CANTIDAD_EMBOTELLADORAS; i++)
    {
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./embotelladora", "embotelladora", ss.str().c_str(), NULL);
            perror("embotelladora - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("embotelladora - fork: ");
        }
        ss.str("");
    }
}