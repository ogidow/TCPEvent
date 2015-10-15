#include <iostream>
#include "TCPEventServer.h"

int main(void)
{

	eventTCP tcp(5432);

	tcp.on("connect", [](Socket *sock){
		std::cout << "connect" << std::endl;
		sock->on("echo", [sock](std::string data){
			std::cout << data << std::endl;
			sock->emit("echo", data);
		});

		sock->on("disconnect", [sock](std::string data){
			std::cout << "disconnect" << std::endl;
			sock->close();
		});
	});

	tcp.listen(5);
	
	getchar();
	return 0;
}