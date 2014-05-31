#ifndef Helper_H
#define	Helper_H
#include "includes.h"
#include <string>
#include <cstdlib>
#include <ctime>

#define NORMAL "\033[0m"

#define WHITE "\033[30m"

#define D_RED "\033[31m"
#define RED "\033[1;31m"
#define BG_RED "\033[41m"

#define D_GREEN "\033[32m"
#define GREEN "\033[1;32m"
#define BG_GREEN "\033[42m"

#define BROWN "\033[33m"
#define YELLOW "\033[1;33m"
#define BG_YELLOW "\033[43m"

#define D_BLUE "\033[34m"
#define BLUE "\033[1;34m"
#define BG_BLUE "\033[44m"

#define PURPLE "\033[35m"
#define PINK "\033[1;35m"
#define BG_PURPLE "\033[45m"

#define D_CYAN "\033[36m"
#define CYAN "\033[1;36m"
#define BG_CYAN "\033[46m"

class Helper {
public:

    Helper() {
        srand(time(NULL));
    };
    static void output(FILE*, std::stringstream&, std::string colour = NORMAL);
    static void output(FILE*, std::string, std::string colour = NORMAL);
    static unsigned int doSleep(int min, int max);
private:

};

#endif	/* Helper_H */

