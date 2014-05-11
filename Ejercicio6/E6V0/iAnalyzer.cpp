#include "iAnalyzer.h"
#include "Helper.h"

#include <sstream>

iAnalyzer::iAnalyzer(long number)
{
    std::stringstream ss;
    this->id = M_ANALYZER + number;
    ss << "iAnalyzer " << number << " (" << this->id << ")";
    this->owner = ss.str();
    this->number = number;
    in = new Queue<struct iMessage>(PATH, Q_ANALYZER_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_ANALYZER_FROM_INTERFACE, owner);
    out->get();
}

struct sample iAnalyzer::getSample(void)
{
    std::stringstream ss;
    struct iMessage msg;
    msg.type = M_ANALYZER;
    msg.sender = this->id;
    msg.message = GET_SAMPLE;
    msg.data.number = this->number;

    out->send(msg);
//    ss << owner << " envie " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
    msg = in->receive(this->id);
//    ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != GOT_SAMPLE)
    {
        ss << owner << " \033[41m\033[30mError\33[0m recibi " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(GOT_SAMPLE) << std::endl;
        Helper::output(stderr, ss);
    }
    return msg.data.sample;

}