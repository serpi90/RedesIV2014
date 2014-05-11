#include "iSampler.h"
#include "Helper.h"

#include <sstream>

iSampler::iSampler(long number)
{
    std::stringstream ss;
    this->id = M_SAMPLER + number;
    ss << "iSampler " << number << " (" << this->id << ")";
    this->owner = ss.str();
    in = new Queue<struct iMessage>(PATH, Q_SAMPLER_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_SAMPLER_FROM_INTERFACE, owner);
    out->get();
}

void iSampler::placeSample(struct sample sample)
{
    std::stringstream ss;
    struct iMessage msg;
    msg.type = M_SAMPLER;
    msg.sender = this->id;
    msg.message = PLACE_SAMPLE;
    msg.data.sample = sample;

    out->send(msg);
//    ss << owner << " envie " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
    msg = in->receive(this->id);
//    ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != SAMPLE_PLACED)
    {
        ss << owner << " \033[41m\033[30mError\33[0m recibi " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(SAMPLE_PLACED) << std::endl;
        Helper::output(stderr, ss);
    }
}

