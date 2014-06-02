#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
int main(int argc, char * argv[])
{

    if (argc < 2)
    {
        std::cerr << "ERROR: usage " << argv[0] << " <amount>" << std::endl;
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    int amount = atoi(argv[1]);
    std::string number;
    for (int i = 0; i < amount; i++)
    {
        std::stringstream ss;
        ss << i;
        number = ss.str();
        pid = fork();
        if (pid == 0)
        {
            execlp("./persona", "persona", number.c_str(), NULL);
            perror("Persona - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("Persona - fork: ");
        }
    }
}
