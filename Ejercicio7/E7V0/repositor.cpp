#include "iRepositor.h"
#include "Helper.h"
#include "Config.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

int main()
{
    std::stringstream ss;
    iRepositor * i = new iRepositor();
    Config conf("config.conf");
    unsigned tiempo;
    unsigned cantidad, max, min;
    enum materiales material;
    max = conf.getInt("repositor cantidad max", 10);
    min = conf.getInt("repositor cantidad min", 4);
    srand(time(NULL));
    while (true)
    {
        Helper::output(stdout, "Repositor: esperando un pedido.\n");
        material = i->esperarPedido();

        cantidad = min + rand() % (max - min + 1);
        ss << "Repositor: recibi pedido de " << Helper::msgToString(material) << " voy a entregar " << cantidad << std::endl;
        Helper::output(stdout, ss);

        tiempo = Helper::doSleep(conf.getInt("tiempo reposicion min", 2), conf.getInt("tiempo reposicion max", 20));
        ss << "Repositor: tarde " << tiempo << " en reponer " << cantidad << " de " << Helper::msgToString(material) << std::endl;
        Helper::output(stdout, ss);

        i->responderPedido(cantidad);
    }
    return 0;
}
