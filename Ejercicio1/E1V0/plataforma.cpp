/* 
 * File:   plataforma.cpp
 * Author: lubuntu
 *
 * Created on April 6, 2014, 2:20 PM
 */

#include <cstdlib>
#include "Plataforma.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    std::stringstream ss;
    if (argc < 2)
    {
        ss << "Usage " << argv[0] << " <a|b> <id>" << std::endl;
        if (write(fileno(stderr), ss.str().c_str(), ss.str().size()))
        {
        }
        exit(EXIT_FAILURE);
    }
    int number = atoi(argv[2]);
    char rtype = argv[1][0];
    Plataforma * p = new Plataforma(number, rtype);
    p->init();
    return 0;
}

