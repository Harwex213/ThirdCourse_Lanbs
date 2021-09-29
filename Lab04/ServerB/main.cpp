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
    cout << "Start searching for other server with \"" << callSign << "\" call sign" << endl;

    try {
        if ((broadcastSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw Error::SetErrorMsgText("Open broadcast Socket: ", WSAGetLastError());

        int broadcastOption = 1;
        int timeoutOption = 2500;
        if (setsockopt(broadcastSocket,
                       SOL_SOCKET,
                       SO_BROADCAST,
                       (char *) &broadcastOption,
                       sizeof(int)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Set broadcast socket option: ", WSAGetLastError());

        if (setsockopt(broadcastSocket,
                       SOL_SOCKET,
                       SO_RCVTIMEO,
                       (char *) &timeoutOption,
                       sizeof(long)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Set socket timeout option: ", WSAGetLastError());

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
            throw Error::SetErrorMsgText("Send broadcast msg: ",WSAGetLastError());

        while(true) {
            if (recvfrom(broadcastSocket,
                         answer,
                         sizeof(answer),
                         NULL,
                         (sockaddr *) &broadcastSocketInfo,
                         &broadcastSocketInfoLength) == SOCKET_ERROR)
                throw Error::SetErrorMsgText("Receive broadcast msg: ",WSAGetLastError());

            cout << "!!!Attention!!!" << endl
                 << "Was found another server with such call sign: "
                    << inet_ntoa(broadcastSocketInfo.sin_addr)
                    << ":" << htons(broadcastSocketInfo.sin_port)
                    << endl;
        }
    }
    catch (string& errorMsgText) {
        cout << "WSAGetLastError: " << errorMsgText << endl;;
    }

    if (closesocket(broadcastSocket) == SOCKET_ERROR)
        throw Error::SetErrorMsgText("Close broadcast Socket: ", WSAGetLastError());
    cout << "Searching ended" << endl;
}

int main()
{
    try {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw Error::SetErrorMsgText("Startup: ", WSAGetLastError());

        const char* callSign = "Hello";
        FindAnotherServers((char *)callSign);

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