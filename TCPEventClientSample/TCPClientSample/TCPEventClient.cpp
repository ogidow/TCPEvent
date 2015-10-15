#include "TCPEventClient.h"


TCPEventClient::TCPEventClient(void)
{
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	sock = socket(AF_INET, SOCK_STREAM, 0);
}


TCPEventClient::~TCPEventClient(void)
{
	th.join();
	WSACleanup();
}


void TCPEventClient::connect(std::string ip, int port)
{
	th = std::thread([this, ip, port](){
		_connect(ip, port);
	});	
}

void TCPEventClient::_connect(std::string ip, int port)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

	::connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	sockObj.open(sock);
	waitForDefineEvent("connect");
	eventList["connect"](&sockObj);
	sockObj.receiving();

}

void TCPEventClient::on(std::string eventName, std::function<void(Socket *)> e)
{
	eventList[eventName] = e;

}

void TCPEventClient::waitForDefineEvent(std::string eventName)
{
	while (true){
		if (eventList.count(eventName) != 0) break;
		Sleep(1000);
	}
	return;
}

void TCPEventClient::close()
{
	sockObj.close();
}