#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

namespace connectionCustomizer
{
    struct ConnectionCustomizer
    {
        SOCKET serverSocket;
        char* callSign;
        SOCKADDR_IN* clientSocketInfo;
        int* clientSocketInfoLength;

        ConnectionCustomizer(SOCKET serverSocket,
                             char* callSign,
                             SOCKADDR_IN* clientSocketInfo,
                             int* clientSocketInfoLength);

        bool IsExistRequestOnConnection();
        bool EstablishConnection();
    };
}