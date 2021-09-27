#include <iostream>
#include "Winsock2.h"
#include "./../../include/error/error.h"
#include "connectionCustomizer.h"

namespace connectionCustomizer
{
    bool EstablishConnection(short port, char* serverCallSign, SOCKADDR_IN& serverSocketInfo)
    {
        memset(&serverSocketInfo, 0, sizeof(serverSocketInfo));
        int serverSocketInfoLength = sizeof(serverSocketInfo);

        SOCKET broadcastSocket;
        if ((broadcastSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw Error::SetErrorMsgText("Open broadcast Socket:", WSAGetLastError());

        int optionValue = 1;
        if (setsockopt(broadcastSocket, SOL_SOCKET, SO_BROADCAST, (char*)&optionValue, sizeof(int) == SOCKET_ERROR))
            throw Error::SetErrorMsgText("Set broadcast socket option:", WSAGetLastError());

        SOCKADDR_IN broadcastSocketInfo;
        broadcastSocketInfo.sin_family = AF_INET;
        broadcastSocketInfo.sin_port = htons(port);
        broadcastSocketInfo.sin_addr.S_un.S_addr = INADDR_BROADCAST;

        if (sendto(broadcastSocket,
                   serverCallSign,
                   strlen(serverCallSign) + 1,
                   NULL,
                   (sockaddr *) &broadcastSocketInfo,
                   sizeof(broadcastSocketInfo)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Send:", WSAGetLastError());

        char answer[50];
        if (recvfrom(broadcastSocket,
                     answer,
                     sizeof(answer),
                     NULL,
                     (sockaddr *) &serverSocketInfo,
                     &serverSocketInfoLength) == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            if (errorCode == WSAETIMEDOUT) {
                return false;
            }

            throw Error::SetErrorMsgText("Receive:", errorCode);
        }

        if (strcmp(serverCallSign, answer) != 0) {
            return false;
        }

        return true;
    }
}