#include "Net.h"

namespace Net
{
    bool connectToServer(const char* ip, uint16_t port, TCPsocket& outSocket, IPaddress& outIPaddress)
    {
        // Getting ip
        if (SDLNet_ResolveHost(&outIPaddress, ip, port) == -1)
        {
            std::cerr << "Resolve host error: " << SDLNet_GetError() << '\n';
            return false;
        }
	
        // Creating socket
        outSocket = SDLNet_TCP_Open(&outIPaddress);
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

    bool sendMessageToServer(const std::string& message, const TCPsocket& serverSocket)
    {
        return sendMessage(message, serverSocket);
    }

    bool sendMessage(const std::string& message, const TCPsocket socket)
    {
        if (sendTo(socket, message))
        {
            const std::string response = receiveFrom(socket);
            if (response.empty() || response != "ack") return false;
            return true;
        }
        return false;
    }
}
