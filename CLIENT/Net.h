#pragma once
#include <iostream>
#include <raylib.h>
#include <SDL_net.h>
#include <string>

namespace Net
{
    inline bool sendTo(const TCPsocket dest, const std::string& msg)
    {
        const int bytesSent = SDLNet_TCP_Send(dest, msg.c_str(), msg.length() + 1);
        if (bytesSent < msg.length()+ 1) {
            std::cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << '\n';
            return false;
        }
        return true;
    }

    inline std::string receiveFrom(const TCPsocket dest)
    {
        char buffer[4096];
        const int bytesRead = SDLNet_TCP_Recv(dest, buffer, sizeof(buffer));
        if (bytesRead > 0) return buffer;
        return "";
    }
    
    bool connectToServer(const char* ip, uint16_t port, TCPsocket& outSocket, IPaddress& outIpAddress);
    bool sendMessage(const std::string& message, TCPsocket socket);
    void closeConnection(const TCPsocket& socket);
    
    bool sendMessageToServer(const std::string& message, const TCPsocket& serverSocket);
};
