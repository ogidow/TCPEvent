#include <iostream>
#include "TCPEventClient.h"

int main(void)
{
	TCPEventClient client;

	client.connect("127.0.0.1", 5432);

	client.on("connect", [](Socket *sock){
		std::cout << "connect" << std::endl;
		sock->on("echo", [sock](std::string data){
			std::cout << data << std::endl;
		});

		sock->on("disconnect", [sock](std::string data){
			sock->close();
		});

		sock->emit("echo", "echo server");
	});

	return 0;
}

