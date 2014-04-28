#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
int main(int argc, char * argv[] )
{
    std::stringstream ss;
    if ( argc < 2 ) {
        std::cerr << "ERROR: usage " << argv[0] << " <amount>" << std::endl;
        exit( EXIT_FAILURE );
    }

    pid_t pid;
    int amount = atoi( argv[1] );
    std::string number;
    for( int i = 0 ; i < amount ; i++ ) {
	ss << i;
        number = ss.str( );
	ss.str("");
        pid = fork();
        if (pid == 0)
        {
            execlp("./persona", "persona", number.c_str(),  NULL);
            perror("Persona - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("Persona - fork: ");
        }
    }
}
