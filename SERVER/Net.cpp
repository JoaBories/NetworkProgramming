#include "Net.h"

namespace Net
{
    bool sendVerification(const TCPsocket& clientSocket, const bool verif)
    {
        const std::string ack = verif ? "ack" : "out";
        return sendTo(clientSocket, ack);
    }
}
