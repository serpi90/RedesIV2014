#include <cstdlib>
#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include "Config.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    Queue<struct personMessage> * in, * salaArriba, * salaAbajo, * cableCarril;
    std::stringstream ss;
    std::string owner;
    struct personMessage msg;

    if (argc < 2)
    {
        ss << "Usage: " << argv[0] << " <id>" << std::endl;
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    int number = atoi(argv[1]);
    long id = M_PERS + number;
    srand(time(NULL) + number);
    long puertaEntrada = M_PUERTA_ABAJO + rand() % DOOR_AMOUNT;
    long puertaSalida = M_PUERTA_ARRIBA + rand() % DOOR_AMOUNT;
    ss << "Persona " << number << " (id: " << (id) << ")";
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

    // Entrar a la sala de abajo
    msg.type = puertaEntrada;
    msg.sender = id;
    msg.message = QUIERO_ENTRAR;
    salaAbajo->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << " a puerta " << msg.type - M_PUERTA_ABAJO << " (" << msg.type << ")" << std::endl;
    Helper::output(stdout, ss);

    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << " de puerta " << msg.sender - M_PUERTA_ABAJO << " (" << msg.sender << ")" << std::endl;
    Helper::output(stdout, ss);

    // Subir al cablecarril (abajo)
    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << " de cablecarril " << msg.sender - M_CABLE_CARRIL << " (" << msg.sender << ")" << std::endl;
    Helper::output(stdout, ss);
    ss << owner << " subi durante: " << Helper::doSleep(conf.getInt("subir cc min", 1), conf.getInt("subir cc max", 1)) << " segundos" << std::endl;
    Helper::output(stdout, ss);

    msg.type = msg.sender;
    msg.sender = id;
    msg.message = SUBI;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << " a cablecarril " << msg.type - M_CABLE_CARRIL << " (" << msg.type << ")" << std::endl;
    Helper::output(stdout, ss);

    // Bajar del cablecarril (arriba)
    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << " de cablecarril " << msg.sender - M_CABLE_CARRIL << " (" << msg.sender << ")" << std::endl;
    Helper::output(stdout, ss);
    ss << owner << " baje durante: " << Helper::doSleep(conf.getInt("bajar cc min", 1), conf.getInt("bajar cc max", 1)) << " segundos" << std::endl;
    Helper::output(stdout, ss);

    msg.type = msg.sender;
    msg.sender = id;
    msg.message = BAJE;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << " a cablecarril " << msg.type - M_CABLE_CARRIL << " (" << msg.type << ")" << std::endl;
    Helper::output(stdout, ss);

    // Pasear arriba
    ss << owner << " paseando" << std::endl;
    Helper::output(stdout, ss);
    ss << owner << " pasee arriba por: " << Helper::doSleep(conf.getInt("paseo arriba min", 1), conf.getInt("paseo arriba max", 1)) << " segundos" << std::endl;
    Helper::output(stdout, ss);

    // Entrar a la sala de arriba
    msg.type = puertaSalida;
    msg.sender = id;
    msg.message = QUIERO_ENTRAR;
    salaArriba->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << " a puerta " << msg.type - M_PUERTA_ARRIBA << " (" << msg.type << ")" << std::endl;
    Helper::output(stdout, ss);

    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << " de puerta " << msg.sender - M_PUERTA_ARRIBA << " (" << msg.sender << ")" << std::endl;
    Helper::output(stdout, ss);

    // Subir al cablecarril (arriba)
    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << " de cablecarril " << msg.sender - M_CABLE_CARRIL << " (" << msg.sender << ")" << std::endl;
    Helper::output(stdout, ss);
    ss << owner << " subi durante: " << Helper::doSleep(conf.getInt("subir cc  min", 1), conf.getInt("subir cc max", 1)) << " segundos" << std::endl;
    Helper::output(stdout, ss);

    msg.type = msg.sender;
    msg.sender = id;
    msg.message = SUBI;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << " a cablecarril " << msg.type - M_CABLE_CARRIL << " (" << msg.type << ")" << std::endl;
    Helper::output(stdout, ss);

    // Bajar del cablecarril (abajo)
    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << " de cablecarril " << msg.sender - M_CABLE_CARRIL << " (" << msg.sender << ")" << std::endl;
    Helper::output(stdout, ss);
    ss << owner << " baje durante: " << Helper::doSleep(conf.getInt("bajar cc min", 1), conf.getInt("bajar cc max", 1)) << " segundos" << std::endl;
    Helper::output(stdout, ss);

    msg.type = msg.sender;
    msg.sender = id;
    msg.message = BAJE;
    cableCarril->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << " a cablecarril " << msg.type - M_CABLE_CARRIL << " (" << msg.type << ")" << std::endl;
    Helper::output(stdout, ss);

    // Bye Bye
    return 0;
}
