#ifndef Helper_H
#define	Helper_H
#include "includes.h"
#include <string>
#include <cstdlib>
#include <ctime>

class Helper {
public:

    Helper() {
        srand(time(NULL));
    };
    static std::string msgToString(enum iMessageType);
    static std::string msgToString(enum analyisisStatus);
    static std::string msgToString(enum workingStatus);
    static void output(FILE*, std::stringstream&, std::string colour = "\033[0m");
    static void output(FILE*, std::string, std::string colour = "\033[0m");
    static unsigned int doSleep(int min, int max);
private:

};

#endif	/* Helper_H */

