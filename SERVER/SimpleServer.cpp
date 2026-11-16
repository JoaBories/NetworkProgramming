#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>

#include "Net.h"

//======================================
//  _____ ___________ _   _ ___________ 
// /  ___|  ___| ___ \ | | |  ___| ___ \
// \ `--.| |__ | |_/ / | | | |__ | |_/ /
//  `--. \  __||    /| | | |  __||    / 
// /\__/ / |___| |\ \\ \_/ / |___| |\ \ 
// \____/\____/\_| \_|\___/\____/\_| \_|
//======================================

int main(int argc, char* argv[]) {
	uint16_t usingPort = 4242;
	
	if (argc > 1)
	{
		uint32_t port = 0xFFFFFFFF;
		try { port = std::stoi(argv[1]); }
		catch (std::exception& e) { std::cerr << "Bad port parameter: " << e.what() << '\n';	}
		
		if (port > 65535 && port != 0xFFFFFFFF) std::cerr << "Retry with a port below 65535 " << '\n';
		else usingPort = (uint16_t)port;
	}
	
	std::cout << "Using port: " << usingPort << '\n';
	
	if (SDLNet_Init() == -1) {
		std::cerr << "SDLNet_Init error: " << SDLNet_GetError() << '\n';
		return 1;
	}

	std::cout << "Kili Chat server init !\n";
	
	// Getting own ip
	IPaddress serverIp;
	if (SDLNet_ResolveHost(&serverIp, nullptr, usingPort) == -1)
	{
		std::cerr << "Resolve host error: " << SDLNet_GetError() << '\n';
		SDLNet_Quit();
		return 1;
	}
	
	// Opening socket
	TCPsocket serverSocket = SDLNet_TCP_Open(&serverIp);
	if (!serverSocket)
	{
		std::cerr << "TCP Open socket error: " << SDLNet_GetError() << '\n';
		SDLNet_Quit();
		return 1;
	}
	
	// Getting client
	TCPsocket clientSocket;
	while (true)
	{
		if (Net::tryReceiveMessage(serverSocket, clientSocket))
		{
			Net::sendVerification(clientSocket);
		}
	}
}