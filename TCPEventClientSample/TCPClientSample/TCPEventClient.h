#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <WinSock2.h>
#include <vector>
#include <functional>
#include <map>

#include "Socket.h"

class TCPEventClient
{
public:
	TCPEventClient(void);
	~TCPEventClient(void);
	void connect(std::string ip, int port);
	void on(std::string eventName, std::function<void(Socket *)> e);
	void close();

private:
	WSADATA wsaData;
	SOCKET sock;
	struct sockaddr_in addr;
	Socket sockObj;
	void _connect(std::string ip, int port);
	std::thread th;
	void waitForDefineEvent(std::string eventName);
	
	std::map<std::string, std::function<void(Socket *)>> eventList;

};

