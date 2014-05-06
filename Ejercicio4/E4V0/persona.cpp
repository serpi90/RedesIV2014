#include <cstdlib>
#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include "Config.h"

using namespace std;

int main(int argc, char** argv)
{
    Queue<struct personMessage> * in, * salaArriba, * salaAbajo, * cableCarril;
    stringstream ss;
    string owner;
    int id;
    struct personMessage msg;

    if (argc < 2)
    {
        ss << "Usage: " << argv[0] << " <id>" << endl;
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    id = atoi(argv[1]);
    ss << "Persona " << id << "(" << (M_PERS + id) << ")";
    owner = ss.str();
    ss.str("");

    Config conf("persona.conf");

    in = new Queue<struct personMessage>(PATH, Q_PERSONA, owner);
    in->get();
    salaAbajo = new Queue<struct personMessage>(PATH, Q_SALA_ABAJO, owner);
    salaAbajo->get();
    salaArriba = new Queue<struct personMessage>(PATH, Q_SALA_ARRIBA, owner);
    salaArriba->get();
    cableCarril = new Queue<struct personMessage>(PATH, Q_CC, owner);
    cableCarril->get();

    msg.type = M_SALA_ABAJO;
    msg.sender = M_PERS + id;
    msg.message = QUIERO_ENTRAR;
    salaAbajo->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    in->receive(&msg, M_PERS + id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    in->receive(&msg, M_PERS + id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);
    Helper::doSleep(conf.getInt("subir cc min",1), conf.getInt("subir cc max",1));

    msg.type = msg.sender;
    msg.sender = M_PERS + id;
    msg.message = SUBI;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    in->receive(&msg, M_PERS + id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);
    Helper::doSleep(conf.getInt("bajar cc min",1), conf.getInt("bajar cc max",1));

    msg.type = msg.sender;
    msg.sender = M_PERS + id;
    msg.message = BAJE;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    Helper::doSleep(conf.getInt("paseo arriba min",1), conf.getInt("paseo arriba max",1));

    msg.type = M_SALA_ARRIBA;
    msg.sender = M_PERS + id;
    msg.message = QUIERO_ENTRAR;
    salaArriba->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    in->receive(&msg, M_PERS + id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    in->receive(&msg, M_PERS + id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);
    Helper::doSleep(conf.getInt("subir cc min",1), conf.getInt("subir cc max",1));

    msg.type = msg.sender;
    msg.sender = M_PERS + id;
    msg.message = SUBI;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    in->receive(&msg, M_PERS + id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);
    Helper::doSleep(conf.getInt("bajar cc min",1), conf.getInt("bajar cc max",1));

    msg.type = msg.sender;
    msg.sender = M_PERS + id;
    msg.message = BAJE;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << endl;
    Helper::output(stdout, ss);

    return 0;
}
