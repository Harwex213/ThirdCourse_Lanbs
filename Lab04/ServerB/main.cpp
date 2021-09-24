#include <iostream>
#include "Winsock2.h"
#include "../include/error/error.h"
#include "./connection/connectionCustomizer.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;
using namespace connectionCustomizer;

int main()
{
    try {
        SOCKET serverSocket;
        WSADATA wsaData;
        SOCKADDR_IN serverSocketInfo;
        serverSocketInfo.sin_family = AF_INET;
        serverSocketInfo.sin_port = htons(2000);
        serverSocketInfo.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw Error::SetErrorMsgText("Startup: ", WSAGetLastError());
        if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw Error::SetErrorMsgText("OpenSocket:",WSAGetLastError());
        if (bind(serverSocket, (LPSOCKADDR) &serverSocketInfo, sizeof(serverSocketInfo)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Bind: ", WSAGetLastError());

        cout << "server is open on: "
             << inet_ntoa(serverSocketInfo.sin_addr)
             << ":" << htons(serverSocketInfo.sin_port)
             << endl;

        const char* callSign = "Hello";
        SOCKADDR_IN clientSocketInfo;
        int clientSocketInfoLength = sizeof(clientSocketInfo);
        ConnectionCustomizer* connectionCustomizer = new ConnectionCustomizer(serverSocket,
                                                                              (char *)callSign,
                                                                              &clientSocketInfo,
                                                                              &clientSocketInfoLength);

        while(true)
        {
            if(connectionCustomizer->IsExistRequestOnConnection())
            {
                if (!connectionCustomizer->EstablishConnection())
                    throw Error::SetErrorMsgText("OpenSocket:",WSAGetLastError());

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
}