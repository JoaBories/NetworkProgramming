#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Net.h"

//======================================
//  _____ ___________ _   _ ___________ 
// /  ___|  ___| ___ \ | | |  ___| ___ \
// \ `--.| |__ | |_/ / | | | |__ | |_/ /
//  `--. \  __||    /| | | |  __||    / 
// /\__/ / |___| |\ \\ \_/ / |___| |\ \ 
// \____/\____/\_| \_|\___/\____/\_| \_|
//======================================

struct messageLog
{
	std::string message;
	std::string pseudo;
};

IPaddress serverIp;
TCPsocket serverSocket;
uint16_t usingPort = 4242;
SDLNet_SocketSet socketSet;

std::vector<messageLog> Log;
std::unordered_map<std::string, TCPsocket> Users;

void sendAll(const std::string& message);
void disconnect(const TCPsocket& clientSocket, const std::string& pseudo);

int main(int argc, char* argv[])
{
	std::cout << "Starting server...\n";
	std::cout << "Enter port:";
	
	{
		std::string portInput;
		std::cin >> portInput;
		
		uint16_t port = 0xFFFF;
		bool error = false;
		try { port = std::stoi(portInput); }
		catch (std::exception& e)
		{
			std::cerr << "Bad port parameter: " << e.what() << '\n';
			std::cout << "Will use default port.\n";
			error = true;
		}
		
		if (!error) usingPort = port;
	}
	
	std::cout << "Using port: " << usingPort << '\n';
	
	if (SDLNet_Init() == -1) {
		std::cerr << "SDLNet_Init error: " << SDLNet_GetError() << '\n';
		return 1;
	}

	std::cout << "Kili Chat server init !\n";
	
	// Getting own ip
	if (SDLNet_ResolveHost(&serverIp, nullptr, usingPort) == -1)
	{
		std::cerr << "Resolve host error: " << SDLNet_GetError() << '\n';
		SDLNet_Quit();
		return 1;
	}
	
	// Opening socket
	serverSocket = SDLNet_TCP_Open(&serverIp);
	if (!serverSocket)
	{
		std::cerr << "TCP Open socket error: " << SDLNet_GetError() << '\n';
		SDLNet_Quit();
		return 1;
	}
	
	std::cout << "connect on local network using: \n";
	std::cout << SDLNet_ResolveIP(&serverIp) << ":" << std::to_string(usingPort) << '\n';
	
	socketSet = SDLNet_AllocSocketSet(8);
	
	while (true)
	{
		{ // newcomer acceptation
			const TCPsocket clientSocket = SDLNet_TCP_Accept(serverSocket);
			if (clientSocket) {
				std::cout << "A client reached the server!\n";
			
				std::string pseudo = Net::receiveFrom(clientSocket);
				if (!pseudo.empty() && !Users.contains(pseudo))
				{
					bool serverFull = SDLNet_TCP_AddSocket(socketSet, clientSocket) == -1;
					if (!serverFull)
					{
						Users[pseudo] = clientSocket;
						Net::sendVerification(clientSocket, true);
						std::cout << pseudo << " has joined !\n";
						sendAll(pseudo + " has joined !");
					}
					else
					{
						Net::sendVerification(clientSocket, false);
					}
				}
				else
				{
					Net::sendVerification(clientSocket, false);
				}
			}
		}
		
		{ // client inputs
			if (SDLNet_CheckSockets(socketSet, 1)!=0 && !Users.empty())
			{
				for (const auto& [pseudo, socket] : Users)
				{
					if (SDLNet_SocketReady(socket))
					{
						std::string msg = Net::receiveFrom(socket);
						if (msg.empty()) break;
						std::cout << pseudo << " -: " << msg << '\n';
						
						if (msg[0] == '/')
						{
							const size_t firstSpace = msg.find_first_of(' ');
							std::string command;
							std::string param;

							if (firstSpace != std::string::npos)
							{
								command = msg.substr(0, firstSpace);
								param = msg.substr(firstSpace+1);
							}
							else
							{
								command = msg;
								param = "";
							}

							if (command == "/disconnect")
							{
								Net::sendVerification(socket, true);
								disconnect(socket, pseudo);
							}
						}
						else
						{
							Net::sendVerification(socket, true);
							sendAll(pseudo + ": " + msg);
						}
						
						break;
					}
				}
			}
		}
	}
	
	SDLNet_FreeSocketSet(socketSet);
}

void sendAll(const std::string& message)
{
	for (const auto& [pseudo, socket] : Users)
	{
		Net::sendTo(socket, message);
	}
}

void disconnect(const TCPsocket& clientSocket, const std::string& pseudo)
{
	sendAll(pseudo + " has been diconnected");
	Users.erase(pseudo);
	SDLNet_TCP_DelSocket(socketSet, clientSocket);
}
