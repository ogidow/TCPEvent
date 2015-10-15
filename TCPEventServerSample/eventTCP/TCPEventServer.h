#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <WinSock2.h>
#include <vector>
#include <functional>
#include <map>
#include "Socket.h"


class eventTCP
{
public:
	eventTCP(int port);
	~eventTCP(void);

	void listen(int maxClient);
	
	void on(std::string eventName, std::function<void(Socket *)> e);

private:
	WSADATA wsaData;
	SOCKET serverSock;
	SOCKET clientSock;
	FD_SET fds;
	FD_SET readFds;
	int maxClient;
	std::vector<Socket> clientList;
	Socket s;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	std::map<std::string, std::function<void(Socket *)>> eventList;
	std::vector<std::thread> clientThreads;
	std::thread listenThread;
	
	void _listen(int maxClient);
	void waitForDefinedEvent(std::string eventName);

};

