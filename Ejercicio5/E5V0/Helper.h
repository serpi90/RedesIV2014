#ifndef Helper_H
#define	Helper_H
#include "includes.h"
#include <cstdlib>
#include <ctime>

class Helper {
public:

    Helper() { srand(time(NULL));};
    static std::string msgToString(enum iMessageType);
    static std::string msgToString(enum personMessageType);
    static std::string msgToString(enum state);
    static std::string msgToString(enum location);
    static void output(FILE*, std::stringstream&);
    static void output(FILE*, std::string);
    static unsigned int doSleep( int min, int max );
private:

};

#endif	/* Helper_H */

