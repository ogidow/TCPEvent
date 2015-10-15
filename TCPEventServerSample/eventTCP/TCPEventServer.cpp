#include "TCPEventServer.h"


eventTCP::eventTCP(int port)
{
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock == INVALID_SOCKET){
		std::cout << "socket faild" << std::endl;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	int err = bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (err == SOCKET_ERROR){
		std::cout << "bind error" << std::endl;
	}
}


eventTCP::~eventTCP(void)
{
	closesocket(clientSock);
	closesocket(serverSock);
	WSACleanup();
}

void eventTCP::listen(int maxClient)
{
	
	listenThread = std::thread([this, maxClient](){
		_listen(maxClient);
	});	
	
}

void eventTCP::_listen(int maxClient){
	this->maxClient = maxClient;
	clientList.reserve(maxClient);
	

	::listen(serverSock, this->maxClient);

	FD_ZERO(&readFds);
	FD_SET(serverSock, &readFds);

	int ret;

	while (1){
		memcpy(&fds, &readFds, sizeof(fd_set));
		ret = select(0, &fds, NULL, NULL, NULL);
		
		if (ret == 1){
			
			if (FD_ISSET(serverSock, &fds)){
				struct sockaddr_in addr;
				int len = sizeof(addr);
				SOCKET sock = accept(serverSock, (struct sockaddr *)&addr, &len);
				
				if (sock != INVALID_SOCKET){
					
					Socket s;
					s.open(sock);
					clientList.push_back(s);
					eventList["connect"](&clientList[clientList.size() - 1]);
					
					clientThreads.push_back(std::thread([this]{clientList[clientList.size() - 1].receiving();}));
					
					
				}
			}
		}
	}

}

void eventTCP::on(std::string eventName, std::function<void(Socket *)> e){
	eventList[eventName] = e;
}

void eventTCP::waitForDefinedEvent(std::string eventName)
{
	while (true){
		if (eventList.count(eventName) != 0) break;
		Sleep(1000);
	}
}