#include "Net.h"

namespace Net
{
    bool tryReceiveMessage(const TCPsocket& serverSocket, TCPsocket& outSocket)
    {
        outSocket = SDLNet_TCP_Accept(serverSocket);
        if (outSocket)
        {
            const IPaddress* clientIp = SDLNet_TCP_GetPeerAddress(outSocket);
            std::cout << SDLNet_ResolveIP(clientIp) << " -= " <<
                         receiveFrom(outSocket) << '\n';
            return true;
        }
        
        return false;
    }

    bool sendVerification(const TCPsocket& clientSocket, const bool verif)
    {
        const std::string ack = verif ? "ack" : "out";
        return sendTo(clientSocket, ack);
    }
}
