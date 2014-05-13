#include "Componente.h"
#include <sstream>
#include <string>
#include "Helper.h"

Componente::Componente()
{
    this->owner = "Componente";
    pintar = new Semaphore(PATH, SEM_PINTAR, owner);
    pintar->get();
    pintar2 = new Semaphore(PATH, SEM_PINTAR2, owner);
    pintar2->get();
    secar = new Semaphore(PATH, SEM_SECAR, owner);
    secar->get();
    secar2 = new Semaphore(PATH, SEM_SECAR2, owner);
    secar2->get();
    motor = new Semaphore(PATH, SEM_MOTOR, owner);
    motor->get();
    motor2 = new Semaphore(PATH, SEM_MOTOR2, owner);
    motor2->get();
    interior = new Semaphore(PATH, SEM_INTERIOR, owner);
    interior->get();
    interior2 = new Semaphore(PATH, SEM_INTERIOR2, owner);
    interior2->get();
    exterior = new Semaphore(PATH, SEM_EXTERIOR, owner);
    exterior->get();
    exterior2 = new Semaphore(PATH, SEM_EXTERIOR2, owner);
    exterior2->get();
    in = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    out->get();
    fromRep = new Queue<struct iMessage>(PATH, Q_FROM_IREPOSITOR, owner);
    fromRep->get();
    toRep = new Queue<struct iMessage>(PATH, Q_TO_IREPOSITOR, owner);
    toRep->get();
}

void Componente::init()
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        msg = in->receive(0);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.type) << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("Componente: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case ENTREGAR_CHASIS:
                    msg.message = ENTREGAR_CHASIS_OK;
                    entregarChasis();
                    break;
                case RECIBIR_CHASIS:
                    msg.message = RECIBIR_CHASIS_OK;
                    esperarChasis();
                    break;
                case RECIBIDO_CHASIS:
                    recibidoChasis();
                    exit(EXIT_SUCCESS);
                    break;

                case ENTREGAR_PINTADO:
                    msg.message = ENTREGAR_PINTADO_OK;
                    entregarPintado();
                    break;
                case RECIBIR_PINTADO:
                    msg.message = RECIBIR_PINTADO_OK;
                    esperarPintado();
                    break;
                case RECIBIDO_PINTADO:
                    recibidoPintado();
                    exit(EXIT_SUCCESS);
                    break;

                case ENTREGAR_SECADO:
                    msg.message = ENTREGAR_SECADO_OK;
                    entregarSecado();
                    break;
                case RECIBIR_SECADO:
                    msg.message = RECIBIR_SECADO_OK;
                    esperarSecado();
                    break;
                case RECIBIDO_SECADO:
                    recibidoSecado();
                    exit(EXIT_SUCCESS);
                    break;

                case ENTREGAR_MOTOR:
                    msg.message = ENTREGAR_MOTOR_OK;
                    entregarMotor();
                    break;
                case RECIBIR_MOTOR:
                    msg.message = RECIBIR_MOTOR_OK;
                    esperarMotor();
                    break;
                case RECIBIDO_MOTOR:
                    recibidoMotor();
                    exit(EXIT_SUCCESS);
                    break;

                case ENTREGAR_INTERIORES:
                    msg.message = ENTREGAR_INTERIORES_OK;
                    entregarInteriores();
                    break;
                case RECIBIR_INTERIORES:
                    msg.message = RECIBIR_INTERIORES_OK;
                    esperarInteriores();
                    break;
                case RECIBIDO_INTERIORES:
                    recibidoInteriores();
                    exit(EXIT_SUCCESS);
                    break;

                case REPONER:
                    msg.message = REPONER_OK;
                    msg.data.cantidad = reponer(msg.type, msg.data.aReponer);
                    break;

                default:
                    ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.type) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            out->send(msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << " a " << Helper::msgToString(msg.type) << std::endl;
            Helper::output(stderr, ss, CYAN);
            exit(EXIT_SUCCESS);
        }
    }
}

void Componente::entregarChasis()
{
    pintar->post();
    pintar2->wait();
}

void Componente::esperarChasis()
{
    pintar->wait();
}

void Componente::recibidoChasis()
{
    pintar2->post();
}

void Componente::entregarPintado()
{
    secar->post();
    secar2->wait();
}

void Componente::esperarPintado()
{
    secar->wait();
}

void Componente::recibidoPintado()
{
    secar2->post();
}

void Componente::entregarSecado()
{
    motor->post();
    motor2->wait();
}

void Componente::esperarSecado()
{
    motor->wait();
}

void Componente::recibidoSecado()
{
    motor2->post();
}

void Componente::entregarMotor()
{
    interior->post();
    interior2->wait();
}

void Componente::esperarMotor()
{
    interior->wait();
}

void Componente::recibidoMotor()
{
    interior2->post();
}

void Componente::entregarInteriores()
{
    exterior->post();
    exterior2->wait();
}

void Componente::esperarInteriores()
{
    exterior->wait();
}

void Componente::recibidoInteriores()
{
    exterior2->post();
}

unsigned Componente::reponer(long client, enum materiales m)
{
    struct iMessage msg;
    msg.type = client;
    msg.message = REPONER;
    msg.data.aReponer = m;
    toRep->send(msg);
    msg = fromRep->receive(client);
    return msg.data.cantidad;
}