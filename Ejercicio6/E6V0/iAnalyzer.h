#ifndef IANALYZER_H
#define	IANALYZER_H

#include <string>
#include "Queue.cpp"
#include "includes.h"

class iAnalyzer {
public:
    iAnalyzer(long number);
    struct sample getSample(void);
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
    long number;
    long id;
};

#endif	/* IANALYZER_H */

