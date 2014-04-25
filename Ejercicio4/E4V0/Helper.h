#ifndef Helper_H
#define	Helper_H
#include "includes.h"

class Helper {
public:

    Helper() {
    };
    static std::string msgToString(enum iMessageType);
    static std::string msgToString(enum personMessageType);
    static void output(FILE*, std::stringstream&);
    static void output(FILE*, std::string);

private:

};

#endif	/* Helper_H */

