#ifndef SOCKET_H
#define	SOCKET_H

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <sys/wait.h>
#include <signal.h>

template <typename T>
class Socket
{
public:

    Socket(std::string owner, bool displayErrors)
    {
        this->owner = owner;
        this->displayErrors = displayErrors;
    }

    /* Utilizar el socket en modo activo (cliente). 
     * Este es el socket que inicia la conexion.
     */
    int active(std::string server, unsigned short port)
    {
        this->port = port;
        this->server = server;
        struct hostent *ptr_server;

        // TODO Cambiar gethostbyname que esta obsoleta a getnameinfo.
        ptr_server = gethostbyname(server.c_str());
        if (((char *) ptr_server) == NULL)
        {
            show_error("gethostbyname");
            return -2;
        }

        // Completar los datos del endpoint remoto para luego establecer la conexion con el mismo.
        bzero((void*) &remote_address, sizeof (remote_address));
        remote_address.sin_family = AF_INET;
        remote_address.sin_port = htons(port);
        memcpy(&remote_address.sin_addr, ptr_server->h_addr, ptr_server->h_length);

        // Crear el socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            show_error("socket");
            return -1;
        }

        // Establecer la conexion
        if (connect(sockfd, (struct sockaddr *) &remote_address, sizeof (remote_address)) < 0)
        {
            show_error("connect");
            return -1;
        }

        // Guardar la direccion local en local_address para poder utlizarla si la necesitamos.
        socklen_t size = sizeof (local_address);
        bzero((char *) &local_address, size);
        if (getsockname(sockfd, (struct sockaddr*) &local_address, &size) < 0)
        {
            show_error("getsockname");
            return -1;
        }

        return 0;
    }

    /* Utilizar el socket en modo pasivo (servidor)
     * Este es el socket que va a utilizar doAccept esperando nuevas conexiones.
     */
    int passive(unsigned short port, int backlog = 5)
    {
        this->port = port;

        // Completar los datos del endpoint pasivo.
        bzero((char *) &local_address, sizeof (local_address));
        local_address.sin_family = AF_INET;
        // Usamos INADDR_ANY para escuchar en todas las interfaces
        local_address.sin_addr.s_addr = htonl(INADDR_ANY);
        local_address.sin_port = htons(port);

        // Crear el socket.
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // SOCK_STREAM = TCP
        {
            show_error("socket");
            return -1;
        }

        // Hacer el binding.
        if (bind(sockfd, (struct sockaddr *) &local_address, sizeof (local_address)) < 0)
        {
            show_error("bind");
            return -1;
        }

        // Empezar a dejar en espera el socket, 
        if (listen(sockfd, backlog) < 0)
        {
            show_error("listen");
            return -1;
        }
        return 0;
    }

    /* Bloquearse a la espera de una nueva conexion. */
    Socket * doAccept()
    {
        int newsockfd;
        socklen_t size = sizeof (remote_address);
        bzero((char *) &remote_address, size);
        newsockfd = accept(sockfd, (struct sockaddr *) &remote_address, &size);
        if (newsockfd <= 0)
        {
            error = newsockfd;
            show_error("accept");
            return NULL;
        } else
        {
            return new Socket(newsockfd, owner, displayErrors);
        }
    }

    int send(T * data)
    {
        size_t size = sizeof (T);
        int sentBytes, lastWrite;

        for (sentBytes = 0; size; sentBytes += lastWrite)
        {

            lastWrite = write(sockfd, (void *) (data + sentBytes), size);
            if (lastWrite <= 0)
            {
                show_error("send");
                return sentBytes;
            }
            size -= lastWrite;
        }
        return sentBytes;
    }

    int receive(T * data)
    {
        int readBytes, lastRead;
        size_t size = sizeof (T);

        for (readBytes = 0; size; readBytes += lastRead)
        {
            lastRead = read(sockfd, (void *) (data + readBytes), size);
            if (lastRead <= 0)
            {
                show_error("receive");
                return readBytes;
            }
            size -= lastRead;
        }
        return readBytes;
    }

    int doClose()
    {
        if (close(sockfd) == -1)
        {
            show_error("close");
            return -1;
        }
        return 0;
    }

    int getSockFd()
    {
        return sockfd;
    }

    in_addr getRemoteAddress()
    {
        return remote_address.sin_addr;
    }

    in_addr getLocalAddress()
    {
        return local_address.sin_addr;
    }
    int error;
private:

    Socket(int sockfd, std::string owner, bool displayErrors)
    {
        this->sockfd = sockfd;
        this->owner = owner;
        this->displayErrors = displayErrors;
    }
    int sockfd;
    std::string owner;
    std::string server;
    unsigned short port;
    bool displayErrors;
    struct sockaddr_in local_address;
    struct sockaddr_in remote_address;

    void show_error(std::string where)
    {
        if (displayErrors)
        {
            std::stringstream ss;
            ss << owner << " (" << getpid() << ") SOCKET " << where;
            perror(ss.str().c_str());
        }
    }
};
#endif