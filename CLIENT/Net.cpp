#include "Net.h"

namespace Net
{
    bool connectToServer(const char* ip, uint16_t port, TCPsocket& outSocket)
    {
        IPaddress ipAdress;
        
        // Getting ip
        if (SDLNet_ResolveHost(&ipAdress, ip, port) == -1)
        {
            std::cerr << "Resolve host error: " << SDLNet_GetError() << '\n';
            return false;
        }
	
        // Creating socket
        outSocket = SDLNet_TCP_Open(&ipAdress);
        if (!outSocket)
        {
            std::cerr << "TCP Open socket error: " << SDLNet_GetError() << '\n';
            return false;
        }
        
        return true;
    }

    void closeConnection(const TCPsocket& socket)
    {
        SDLNet_TCP_Close(socket);
    }

    bool sendMessageToServer(const char* ip, const uint16_t port, const std::string& message)
    {
        TCPsocket socket;
        if (connectToServer(ip, port, socket))
        {
            const bool success = sendMessage(message, socket);
            closeConnection(socket);
            return success;
        }
        return false;
    }

    bool sendMessage(const std::string& message, const TCPsocket socket)
    {
        if (sendTo(socket, message))
        {
            std::cout << "me -= " << message << " | ";

            std::string response = receiveFrom(socket);
            if (!response.empty()) std::cout << "sent \n";
            
            return true;
        }
        return false;
    }
}
