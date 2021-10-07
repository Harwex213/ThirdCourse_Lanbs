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
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw Error::SetErrorMsgText("Startup: ", WSAGetLastError());

        const char* callSign = "Hello";

        SOCKET serverSocket;
        if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw Error::SetErrorMsgText("OpenSocket: ",WSAGetLastError());

        SOCKADDR_IN serverSocketInfo;
        serverSocketInfo.sin_family = AF_INET;
        serverSocketInfo.sin_port = htons(2000);
        serverSocketInfo.sin_addr.S_un.S_addr = INADDR_ANY;
        if (bind(serverSocket, (LPSOCKADDR) &serverSocketInfo, sizeof(serverSocketInfo)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Bind: ", WSAGetLastError());

        SOCKADDR_IN clientSocketInfo;
        int clientSocketInfoLength = sizeof(clientSocketInfo);
        ConnectionCustomizer* connectionCustomizer = new ConnectionCustomizer(serverSocket,
                                                                              (char *)callSign,
                                                                              &clientSocketInfo,
                                                                              &clientSocketInfoLength);

        char hostname[50];
        gethostname(hostname, sizeof(hostname));
        cout << "server is open on: "
             << inet_ntoa(serverSocketInfo.sin_addr)
             << ":" << htons(serverSocketInfo.sin_port)
             << endl;
        cout << "\thostname: "
             << hostname
             << endl;

        while(true)
        {
            if(connectionCustomizer->IsExistRequestOnConnection())
            {
                if (!connectionCustomizer->EstablishConnection())
                    throw Error::SetErrorMsgText("Establishing connection: ",WSAGetLastError());

                cout << "Was established connection with "
                     << inet_ntoa(clientSocketInfo.sin_addr)
                     << ":"
                     << htons(clientSocketInfo.sin_port)
                     << endl;
                cout << "\thostname: "
                     << gethostbyaddr((char*)&clientSocketInfo.sin_addr,
                                      sizeof(clientSocketInfo.sin_addr),
                                      AF_INET)->h_name
                     << endl;
            }
        }
    }
    catch (string& errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText << endl;;
    }
    system("pause");
}