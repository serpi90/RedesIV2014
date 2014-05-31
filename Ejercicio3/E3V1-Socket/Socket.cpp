#include "Socket.h"

//Open Pasivo

Socket::Socket(std::string owner, bool displayErrors)
{
    this->owner = owner;
    this->displayErrors = displayErrors;
}

Socket::Socket(int sockfd, struct sockaddr_in local_address, struct sockaddr_in remote_address, std::string owner, bool displayErrors)
{
    this->sockfd = sockfd;
    this->owner = owner;
    this->displayErrors = displayErrors;

    this->local_address = local_address;
    this->remote_address = remote_address;
}

int Socket::passive(unsigned short port)
{
    this->port = port;

    bzero((char *) &local_address, sizeof (local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // SOCK_STREAM = TCP
    {
        show_error("socket");
        return -1;
    }

    if (bind(sockfd, (struct sockaddr *) &local_address, sizeof (local_address)) < 0)
    {
        show_error("bind");
        return -1;
    }

    if (listen(sockfd, 5) < 0)
    {
        show_error("listen");
        return -1;
    }
    return 0;
}

int Socket::active(std::string server, unsigned short port)
{
    this->port = port;
    this->server = server;

    struct hostent *ptr_server;

    ptr_server = gethostbyname(server.c_str());
    if (((char *) ptr_server) == NULL)
    {
        show_error("gethostbyname");
        return -2;
    }

    bzero((char *) &remote_address, sizeof (remote_address));
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
    memcpy(&remote_address.sin_addr, ptr_server->h_addr, ptr_server->h_length);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        show_error("socket");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *) &remote_address, sizeof (remote_address)) < 0)
    {
        show_error("connect");
        return -1;
    }

    socklen_t size = sizeof (local_address);
    bzero((char *) &local_address, size);
    if (getsockname(sockfd, (struct sockaddr*) &local_address, &size) < 0)
    {
        show_error("getsockname");
        return -1;
    }

    return 0;
}

Socket * Socket::doAccept()
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
        return new Socket(newsockfd, local_address, remote_address, owner, displayErrors);
    }
}

int Socket::send(char * data, size_t size)
{
    int sentBytes, lastWrite;

    for (sentBytes = 0; size; sentBytes += lastWrite)
    {

        lastWrite = write(sockfd, (void*) (data + sentBytes), size);
        if (lastWrite <= 0)
        {
            show_error("send");
            return sentBytes;
        }
        size -= lastWrite;
    }
    return sentBytes;
}

int Socket::receive(char * data, size_t size)
{
    int readBytes, lastRead;

    for (readBytes = 0; size; readBytes += lastRead)
    {
        lastRead = read(sockfd, (void*) (data + readBytes), size);
        if (lastRead <= 0)
        {
            show_error("receive");
            return readBytes;
        }
        size -= lastRead;
    }
    return readBytes;
}

void Socket::show_error(std::string where)
{
    if (displayErrors)
    {
        std::stringstream ss;
        ss << owner << " (" << getpid() << ") SOCKET " << where;
        perror(ss.str().c_str());
    }
}

int Socket::doClose()
{
    if (close(sockfd) == -1)
    {
        show_error("close");
        return -1;
    }
    return 0;
}

int Socket::getSockFd()
{
    return sockfd;
}

in_addr Socket::getRemoteAddress()
{
    return remote_address.sin_addr;
}

in_addr Socket::getLocalAddress()
{
    return local_address.sin_addr;
}