#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>

#include "ChatWindow.h"
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

	ServerStatus status { "localhost", 4242, false };

	const auto darkTheme = Theme(
		{25, 25, 25, 255},
		{50, 50, 50, 255},
		RAYWHITE,
		{ 244, 89, 41, 255 }
	);
	
	ChatWindow chatWindow(600, 800, darkTheme, status);
	chatWindow.updateTyping("...");

	std::cout << "Kili Chat client init !\n";
	
	std::string typing;
	
	while (!WindowShouldClose())
	{
		chatWindow.update();
		
		BeginDrawing();
		chatWindow.draw();
		EndDrawing();
		
		int inputChar = GetCharPressed();
		if(inputChar != 0)
		{
			typing += static_cast<char>(inputChar);
			chatWindow.updateTyping(typing);
		}
		if(IsKeyPressed(KEY_BACKSPACE) && !typing.empty())
		{
			typing.pop_back();
			chatWindow.updateTyping(typing);
		}
		
		std::string clientInput;
		auto input = Utils::breakString(clientInput, ' ');
		
		if (input[0] == "close") break;
		else if (input[0] == "ip")
		{
			status.ip = input[1];
			std::cout << "Set server ip to: " << status.ip << '\n';
		}
		else if (input[0] == "port")
		{
			status.port = std::stoi(input[1]);
			std::cout << "Set server port to: " << status.port << '\n';
		}
		else if (input[0] == "server")
		{
			std::cout << "current server: " << status.ip << ":" << status.port << '\n';
		}
		else if (input[0] == "send")
		{
			std::string message = clientInput.substr(clientInput.find_first_of(' '));
			Net::sendMessageToServer(status.ip.c_str(), status.port, message);
		}
	}
	
	SDLNet_Quit();
	CloseWindow();
	
	return 0;
}
