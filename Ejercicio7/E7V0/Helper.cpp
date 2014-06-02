#include <sstream>
#include <cstdio>
#include "Helper.h"
#include <unistd.h>
void Helper::output(FILE* file, std::stringstream &ss, std::string colour)
{
    output(file, ss.str(), colour);
    ss.str("");
}

void Helper::output(FILE* file, std::string s, std::string colour)
{
    s = colour + s + NORMAL;
    if (write(fileno(file), s.c_str(), s.size()))
    {
    }
}

unsigned int Helper::doSleep(int min, int max)
{
    unsigned int time = min + rand() % (max - min + 1);
    sleep(min + rand() % (max - min + 1));
    return time;
}

std::string Helper::msgToString(enum iMessageType m)
{
    switch (m)
    {
        case ENTREGAR_CHASIS:
            return std::string(BROWN) + "ENTREGAR CHASIS" + NORMAL;
            break;
        case ENTREGAR_CHASIS_OK:
            return std::string(BROWN) + "ENTREGAR CHASIS OK" + NORMAL;
            break;
        case RECIBIR_CHASIS:
            return std::string(BROWN) + "RECIBIR CHASIS" + NORMAL;
            break;
        case RECIBIR_CHASIS_OK:
            return std::string(BROWN) + "RECIBIR CHASIS OK" + NORMAL;
            break;
        case RECIBIDO_CHASIS:
            return std::string(BROWN) + "RECIBIDO CHASIS" + NORMAL;
            break;
        case ENTREGAR_PINTADO:
            return std::string(BROWN) + "ENTREGAR PINTADO" + NORMAL;
            break;
        case ENTREGAR_PINTADO_OK:
            return std::string(BROWN) + "ENTREGAR PINTADO OK" + NORMAL;
            break;
        case RECIBIR_PINTADO:
            return std::string(BROWN) + "RECIBIR PINTADO" + NORMAL;
            break;
        case RECIBIR_PINTADO_OK:
            return std::string(BROWN) + "RECIBIR PINTADO OK" + NORMAL;
            break;
        case RECIBIDO_PINTADO:
            return std::string(BROWN) + "RECIBIDO PINTADO" + NORMAL;
            break;
        case ENTREGAR_SECADO:
            return std::string(BROWN) + "ENTREGAR SECADO" + NORMAL;
            break;
        case ENTREGAR_SECADO_OK:
            return std::string(BROWN) + "ENTREGAR SECADO OK" + NORMAL;
            break;
        case RECIBIR_SECADO:
            return std::string(BROWN) + "RECIBIR SECADO" + NORMAL;
            break;
        case RECIBIR_SECADO_OK:
            return std::string(BROWN) + "RECIBIR SECADO OK" + NORMAL;
            break;
        case RECIBIDO_SECADO:
            return std::string(BROWN) + "RECIBIDO SECADO" + NORMAL;
            break;
        case ENTREGAR_MOTOR:
            return std::string(BROWN) + "ENTREGAR MOTOR" + NORMAL;
            break;
        case ENTREGAR_MOTOR_OK:
            return std::string(BROWN) + "ENTREGAR MOTOR OK" + NORMAL;
            break;
        case RECIBIR_MOTOR:
            return std::string(BROWN) + "RECIBIR MOTOR" + NORMAL;
            break;
        case RECIBIR_MOTOR_OK:
            return std::string(BROWN) + "RECIBIR MOTOR OK" + NORMAL;
            break;
        case RECIBIDO_MOTOR:
            return std::string(BROWN) + "RECIBIDO MOTOR" + NORMAL;
            break;
        case ENTREGAR_INTERIORES:
            return std::string(BROWN) + "ENTREGAR INTERIORES" + NORMAL;
            break;
        case ENTREGAR_INTERIORES_OK:
            return std::string(BROWN) + "ENTREGAR INTERIORES OK" + NORMAL;
            break;
        case RECIBIR_INTERIORES:
            return std::string(BROWN) + "RECIBIR INTERIORES" + NORMAL;
            break;
        case RECIBIR_INTERIORES_OK:
            return std::string(BROWN) + "RECIBIR INTERIORES OK" + NORMAL;
            break;
        case RECIBIDO_INTERIORES:
            return std::string(BROWN) + "RECIBIDO INTERIORES" + NORMAL;
            break;
        case REPONER:
            return std::string(BROWN) + "REPONER" + NORMAL;
            break;
        case REPONER_OK:
            return std::string(BROWN) + "REPONER OK" + NORMAL;
            break;
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}

std::string Helper::msgToString(long l)
{
    switch (l)
    {
        case M_CONSTRUCCION:
            return std::string(BLUE) + "CONSTRUCCION" + NORMAL;
        case M_PINTADO:
            return std::string(BLUE) + "PINTADO" + NORMAL;
        case M_SECADO:
            return std::string(BLUE) + "SECADO" + NORMAL;
        case M_COLOCADO_MOTOR:
            return std::string(BLUE) + "COLOCADO_MOTOR" + NORMAL;
        case M_COLOCADO_INTERIOR:
            return std::string(BLUE) + "COLOCADO_INTERIOR" + NORMAL;
        case M_COLOCADO_EXTERIOR:
            return std::string(BLUE) + "COLOCADO_EXTERIOR" + NORMAL;
        default:
            std::stringstream ss;
            ss << BG_RED << WHITE << l << NORMAL;
            return ss.str();
    }
}

std::string Helper::msgToString(enum materiales m)
{
    switch (m)
    {
        case MATERIALES:
            return std::string(D_GREEN) + "MATERIALES" + NORMAL;
            break;
        case PINTURA:
            return std::string(D_GREEN) + "PINTURA" + NORMAL;
            break;
        case MOTOR:
            return std::string(D_GREEN) + "MOTOR" + NORMAL;
            break;
        case INTERIORES:
            return std::string(D_GREEN) + "INTERIORES" + NORMAL;
            break;
        case RUEDA:
            return std::string(D_GREEN) + "RUEDA" + NORMAL;
            break;
        case PUERTA:
            return std::string(D_GREEN) + "PUERTA" + NORMAL;
            break;
        case CAPO:
            return std::string(D_GREEN) + "CAPO" + NORMAL;
            break;
        case BAUL:
            return std::string(D_GREEN) + "BAUL" + NORMAL;
            break;
        default:
            std::stringstream ss;
            ss << BG_RED << WHITE << m << NORMAL;
            return ss.str();
    }
}
