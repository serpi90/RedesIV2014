#ifndef SOCKET_H
#define	SOCKET_H

#include <netinet/in.h>
#include <string>

class Socket {
	public:
		Socket(std::string owner, bool displayErrors = true);

		int active(std::string server, unsigned short port);
		int passive(unsigned short port);
		Socket * doAccept();
		int doClose();
		int send(char * data, size_t size);
		int receive(char * data, size_t size);
		int getSockFd();
		in_addr getRemoteAddress();
		in_addr getLocalAddress();
		int error;
	private:
		Socket(int sockfd, struct sockaddr_in local_address, struct sockaddr_in remote_address, std::string owner, bool displayErrors = true);
		void show_error(std::string where);

		int sockfd;
		std::string owner;
		std::string server;
		bool displayErrors;
		struct sockaddr_in local_address;
		struct sockaddr_in remote_address;
};

#endif

