#include <sstream>

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

std::string Helper::msgToString(enum iMessageType m)
{
    switch (m)
    {
        case ESPERAR_PERSONA_ABAJO:
            return "\033[1;34mESPERAR PERSONA ABAJO\033[0m";
        case ESPERAR_PERSONA_ABAJO_OK:
            return "\033[1;34mESPERAR PERSONA ABAJO OK\033[0m";
        case ESPERAR_PERSONA_ARRIBA:
            return "\033[1;34mESPERAR PERSONA ARRIBA\033[0m";
        case ESPERAR_PERSONA_ARRIBA_OK:
            return "\033[1;34mESPERAR PERSONA ARRIBA OK\033[0m";
        case ESTOY_ABAJO:
            return "\033[1;34mESTOY ABAJO\033[0m";
        case ESTOY_ABAJO_OK:
            return "\033[1;34mESTOY ABAJO OK\033[0m";
        case ESTOY_ARRIBA:
            return "\033[1;34mESTOY ARRIBA\033[0m";
        case ESTOY_ARRIBA_OK:
            return "\033[1;34mESTOY ARRIBA OK\033[0m";
        case CARGAR_PERSONAS_ABAJO:
            return "\033[1;34mCARGAR PERSONAS ABAJO\033[0m";
        case CARGAR_PERSONAS_ABAJO_OK:
            return "\033[1;34mCARGAR PERSONAS ABAJO OK\033[0m";
        case CARGAR_PERSONAS_ARRIBA:
            return "\033[1;34mCARGAR PERSONAS ARRIBA\033[0m";
        case CARGAR_PERSONAS_ARRIBA_OK:
            return "\033[1;34mCARGAR PERSONAS ARRIBA OK\033[0m";
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
            return "\033[1;33mQUIERO ENTRAR\033[0m";
        case ENTRA:
            return "\033[1;33mENTRA\033[0m";
        case SUBIR:
            return "\033[1;33mSUBIR\033[0m";
        case SUBI:
            return "\033[1;33mSUBI\033[0m";
        case BAJAR:
            return "\033[1;33mBAJAR\033[0m";
        case BAJE:
            return "\033[1;33mBAJE\033[0m";
        default:
            std::stringstream ss;
            ss << m;
            return ss.str();
    }
}