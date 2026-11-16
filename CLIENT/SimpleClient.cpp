#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>

#include "Utils.h"
#include "Net.h"

//======================================
//  _____  _     _____ _____ _   _ _____ 
// /  __ \| |   |_   _|  ___| \ | |_   _|
// | /  \/| |     | | | |__ |  \| | | |  
// | |    | |     | | |  __|| . ` | | |  
// | \__/\| |_____| |_| |___| |\  | | |  
//  \____/\_____/\___/\____/\_| \_/ \_/  
//======================================

int main(int argc, char* argv[]) {
	if (SDLNet_Init() == -1) {
		std::cerr << "SDLNet_Init error: " << SDLNet_GetError() << '\n';
		return 1;
	}

	std::cout << "Kili Chat client init !\n";
	
	std::string currentServerIP = "localhost";
	int currentServerPort = 4242;
	
	while (true)
	{
		std::string clientInput;
		std::getline(std::cin, clientInput);
		
		auto input = Utils::breakString(clientInput, ' ');
		
		if (input[0] == "close") break;
		else if (input[0] == "ip")
		{
			currentServerIP = input[1];
			std::cout << "Set server ip to: " << currentServerIP << '\n';
		}
		else if (input[0] == "port")
		{
			currentServerPort = std::stoi(input[1]);
			std::cout << "Set server port to: " << currentServerPort << '\n';
		}
		else if (input[0] == "server")
		{
			std::cout << "current server: " << currentServerIP << ":" << currentServerPort << '\n';
		}
		else if (input[0] == "send")
		{
			std::string message = clientInput.substr(clientInput.find_first_of(' '));
			Net::sendMessageToServer(currentServerIP.c_str(), currentServerPort, message);
		}
	}
	
	SDLNet_Quit();
	
	return 0;
}
