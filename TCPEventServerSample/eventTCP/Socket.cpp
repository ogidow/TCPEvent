#include "Socket.h"


Socket::Socket(void)
{
	
}


Socket::~Socket(void)
{
}

void Socket::open(SOCKET sock)
{
	this->sock = sock;
	status = 1;
}

void Socket::receiving()
{
	
	ParsedData *parsedData = new ParsedData();
	while (1){
		if (status == 0) break;
		
		receiveData(parsedData);
		
		
		if (parsedData->eventName != ""){
			if (eventList.count(parsedData->eventName) != 0){
				eventList[parsedData->eventName](parsedData->data);
				parsedData->eventName.clear();
				parsedData->data.clear();
			}
		} else {
			close();
		}
	}

}


void Socket::on(std::string eventName, std::function<void(std::string)> e)
{
	eventList[eventName] = e;
}

void Socket::receiveData(Socket::ParsedData *parsedData){
	/*
	4bytes : data length
	4bytes : event name length 
	???bytes : event name
	???bytes : data
	*/
	int err;
	int dataSize;
	int nameSize;
	char dataSizeChar[4];
	char nameSizeChar[4];
	char *eventName;
	
	err = recv(sock, dataSizeChar, sizeof(dataSizeChar), 0);
	
	if (err == SOCKET_ERROR) return;
	
	dataSize = (int)(dataSizeChar[0] | dataSizeChar[1] << 8 | dataSizeChar[2] << 16 | dataSizeChar[3] << 24) + 1;
	
	parsedData->dataSize = dataSize;

	err = recv(sock, nameSizeChar, sizeof(nameSizeChar), 0);
	if (err == SOCKET_ERROR) return;

	nameSize = (int)(nameSizeChar[0] | nameSizeChar[1] << 8 | nameSizeChar[2] << 16 | nameSizeChar[3] << 24);

	eventName = new char[nameSize];
	err = recv(sock, eventName, nameSize, 0);
	if (err == SOCKET_ERROR) return;

	parsedData->eventName.insert(0, eventName, nameSize);
	delete[] eventName;

	//parsedData->data = new char[dataSize];
	char *buf = new char[dataSize];
	err = recv(sock, buf, dataSize, 0);
	
	buf[dataSize - 1] = '\0';
	parsedData->data.insert(0, buf, dataSize);
	
	delete[] buf;

	if (err == SOCKET_ERROR) return;

}

void Socket::emit(std::string eventName, std::string data){
	int nameSize = eventName.length();
	int size = 4 + 4 + nameSize + data.length();
	char *sendData = new char[size];

	sendData[0] = data.length() & 0xff;
	sendData[1] = (data.length() & 0xff00) >> 8;
	sendData[2] = (data.length() & 0xff0000) >> 16;
	sendData[3] = (data.length() & 0xff000000) >> 24;

	sendData[4] = nameSize & 0xff;
	sendData[5] = (nameSize & 0xff00) >> 8;
	sendData[6] = (nameSize & 0xff0000) >> 16;
	sendData[7] = (nameSize & 0xff000000) >> 24;

	memcpy(&sendData[8], eventName.c_str(), nameSize);
	memcpy(&sendData[8 + nameSize], data.c_str(), data.length());

	
	send(sock, sendData, size, 0);
	
}

void Socket::close()
{
	
	emit("disconnect", "");
	closesocket(sock);
	sock = INVALID_SOCKET;
	status = 0;
	
//	th.join();

	return;
}