#include <iostream>
#include "Winsock2.h"
#include "../include/error/error.h"
#include "./connection/connectionCustomizer.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;
using namespace connectionCustomizer;

void FindAnotherServers(char* callSign)
{
    SOCKET broadcastSocket;
    if ((broadcastSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw Error::SetErrorMsgText("Open broadcast Socket:", WSAGetLastError());

    int optionValue = 1;
    if (setsockopt(broadcastSocket, SOL_SOCKET, SO_BROADCAST, (char*)&optionValue, sizeof(int) == SOCKET_ERROR))
        throw Error::SetErrorMsgText("Set broadcast socket option:", WSAGetLastError());

    SOCKADDR_IN broadcastSocketInfo;
    broadcastSocketInfo.sin_family = AF_INET;
    broadcastSocketInfo.sin_port = htons(2000);
    broadcastSocketInfo.sin_addr.S_un.S_addr = INADDR_BROADCAST;
    int broadcastSocketInfoLength = sizeof(broadcastSocketInfo);
    char answer[50];

    if (sendto(broadcastSocket,
               callSign,
               strlen(callSign) + 1,
               NULL,
               (sockaddr *) &broadcastSocketInfo,
               sizeof(broadcastSocketInfo)) == SOCKET_ERROR)
        throw Error::SetErrorMsgText("Send broadcast msg:",WSAGetLastError());

    if (recvfrom(broadcastSocket,
                 answer,
                 sizeof(answer),
                 NULL,
                 (sockaddr *) &broadcastSocketInfo,
                 &broadcastSocketInfoLength) == SOCKET_ERROR)
        throw Error::SetErrorMsgText("Receive broadcast msg:",WSAGetLastError());

    cout << answer << endl;

    if (closesocket(broadcastSocket) == SOCKET_ERROR)
        throw Error::SetErrorMsgText("Close broadcast Socket:", WSAGetLastError());
}

int main()
{
    try {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw Error::SetErrorMsgText("Startup: ", WSAGetLastError());

        SOCKET serverSocket;
        if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw Error::SetErrorMsgText("OpenSocket:",WSAGetLastError());

        SOCKADDR_IN serverSocketInfo;
        serverSocketInfo.sin_family = AF_INET;
        serverSocketInfo.sin_port = htons(2000);
        serverSocketInfo.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        if (bind(serverSocket, (LPSOCKADDR) &serverSocketInfo, sizeof(serverSocketInfo)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Bind: ", WSAGetLastError());

        const char* callSign = "Hello";
        SOCKADDR_IN clientSocketInfo;
        int clientSocketInfoLength = sizeof(clientSocketInfo);
        ConnectionCustomizer* connectionCustomizer = new ConnectionCustomizer(serverSocket,
                                                                              (char *)callSign,
                                                                              &clientSocketInfo,
                                                                              &clientSocketInfoLength);

//        FindAnotherServers((char *)callSign);

        cout << "server is open on: "
             << inet_ntoa(serverSocketInfo.sin_addr)
             << ":" << htons(serverSocketInfo.sin_port)
             << endl;

        while(true)
        {
            if(connectionCustomizer->IsExistRequestOnConnection())
            {
                if (!connectionCustomizer->EstablishConnection())
                    throw Error::SetErrorMsgText("Establishing connection:",WSAGetLastError());

                cout << "Was established connection with "
                     << inet_ntoa(clientSocketInfo.sin_addr)
                     << ":"
                     << htons(clientSocketInfo.sin_port)
                     << endl;
            }
        }
    }
    catch (string& errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText << endl;;
    }
    system("pause");
}