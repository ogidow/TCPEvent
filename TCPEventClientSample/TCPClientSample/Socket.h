#pragma once
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <vector>
#include <functional>
#include <map>
#include <mutex>
#include <thread>


class Socket
{

	typedef struct {
		std::string eventName;
		int dataSize;
		std::string data;
	} ParsedData;

public:
	Socket(void);
	~Socket(void);

	void receiving();
	void on(std::string eventName, std::function<void(std::string)> e);
	void emit(std::string eventName, std::string data);
	void open(SOCKET sock);
	void close();
	

private:
	std::map<std::string, std::function<void(std::string)>> eventList;
	SOCKET sock;
	void receiveData(ParsedData *parsedData);
	int status;
	std::thread th;
	std::mutex mtx;
	
	
};

