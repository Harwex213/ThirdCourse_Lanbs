#include <iostream>
#include "Winsock2.h"
#include "../../include/error/error.h"
#include "connectionCustomizer.h"

#pragma comment(lib, "WS2_32.lib")

using namespace std;

namespace connectionCustomizer
{
    ConnectionCustomizer::ConnectionCustomizer(SOCKET serverSocket, char *callSign,
                                               SOCKADDR_IN *clientSocketInfo,
                                               int *clientSocketInfoLength) {
        this->serverSocket = serverSocket;
        this->callSign = callSign;
        this->clientSocketInfo = clientSocketInfo;
        this->clientSocketInfoLength = clientSocketInfoLength;
    }

    bool ConnectionCustomizer::IsExistRequestOnConnection()
    {
        char inputBuffer[50];

        do {
            memset(this->clientSocketInfo, NULL, *this->clientSocketInfoLength);

            if (recvfrom(this->serverSocket,
                         inputBuffer,
                         sizeof(inputBuffer),
                         NULL,
                         (sockaddr *) this->clientSocketInfo,
                         this->clientSocketInfoLength) == SOCKET_ERROR) {
                int errorCode = WSAGetLastError();
                if (errorCode == WSAETIMEDOUT) {
                    return false;
                }

                throw Error::SetErrorMsgText("Receive:", errorCode);
            }
        } while(strcmp(inputBuffer, this->callSign) != 0);

        return true;
    }

    bool ConnectionCustomizer::EstablishConnection()
    {
        return sendto(this->serverSocket,
                      this->callSign,
                      strlen(this->callSign) + 1,
                      NULL,
                      (sockaddr *) this->clientSocketInfo,
                      *this->clientSocketInfoLength) != SOCKET_ERROR;
    }
}