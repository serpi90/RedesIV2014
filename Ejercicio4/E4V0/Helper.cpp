#include <sstream>
#include <unistd.h>
#include "Helper.h"

void Helper::output(FILE* file, std::stringstream &ss)
{
    output(file, ss.str());
    ss.str("");
}

void Helper::output(FILE* file, std::string s)
{
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
        case ESPERAR_PERSONA_ABAJO:
            return "\033[1;34miESPERAR PERSONA ABAJO\033[0m";
        case ENTRO_PERSONA_ABAJO:
            return "\033[1;34miENTRO PERSONA ABAJO\033[0m";
        case ESPERAR_PERSONA_ARRIBA:
            return "\033[1;32miESPERAR PERSONA ARRIBA\033[0m";
        case ENTRO_PERSONA_ARRIBA:
            return "\033[1;32miENTRO PERSONA ARRIBA\033[0m";
        case ESTOY_ABAJO:
            return "\033[1;31miESTOY ABAJO\033[0m";
        case BAJARON_PASAJEROS_ABAJO:
            return "\033[1;31miBAJARON PASAJEROS ABAJO\033[0m";
        case ESTOY_ARRIBA:
            return "\033[1;37miESTOY ARRIBA\033[0m";
        case BAJARON_PASAJEROS_ARRIBA:
            return "\033[1;37miBAJARON PASAJEROS ARRIBA\033[0m";
        case CARGAR_PERSONAS_ABAJO:
            return "\033[1;38miCARGAR PERSONAS ABAJO\033[0m";
        case TERMINE_DE_CARGAR_PERSONAS_ABAJO:
            return "\033[1;38miTERMINE DE CARGAR PERSONAS ABAJO\033[0m";
        case CARGAR_PERSONAS_ARRIBA:
            return "\033[1;39miCARGAR PERSONAS ARRIBA\033[0m";
        case TERMINE_DE_CARGAR_PERSONAS_ARRIBA:
            return "\033[1;39miTERMINE DE CARGAR PERSONAS ARRIBA\033[0m";
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}

std::string Helper::msgToString(enum personMessageType m)
{
    switch (m)
    {
        case QUIERO_ENTRAR:
            return "\033[0;33mQUIERO ENTRAR\033[0m";
        case ENTRA:
            return "\033[1;33mENTRA\033[0m";
        case SUBIR:
            return "\033[1;35mSUBIR\033[0m";
        case SUBI:
            return "\033[0;35mSUBI\033[0m";
        case BAJAR:
            return "\033[1;36mBAJAR\033[0m";
        case BAJE:
            return "\033[0;36mBAJE\033[0m";
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}

std::string Helper::msgToString(enum state s)
{
    switch (s)
    {
        case WAITING: return "WAITING";
        case WORKING: return "WORKING";
        case TRAVELLING: return "TRAVELLING";
        default:
            std::stringstream ss;
            ss << s;
            return ss.str();
    }
}

std::string Helper::msgToString(enum location l)
{
    switch (l)
    {
        case BOTTOM: return "Abajo";
        case TOP: return "Arriba";
        default:
            std::stringstream ss;
            ss << l;
            return ss.str();
    }
}
