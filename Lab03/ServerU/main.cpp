#include <iostream>
#include "../include/error/error.h"
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;

int main() {
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
        if (bind(serverSocket, (LPSOCKADDR)&serverSocketInfo, sizeof(serverSocketInfo)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Bind: ", WSAGetLastError());

        cout << "server is open on: "
             << inet_ntoa(serverSocketInfo.sin_addr)
             << ":" << htons(serverSocketInfo.sin_port)
             << endl;

        SOCKADDR_IN clientSocketInfo;
        int clientSocketInfoLength = sizeof(clientSocketInfo);
        char inputBuffer[50];

        while (true) {
            memset(&clientSocketInfo, 0, clientSocketInfoLength);

            if (recvfrom(serverSocket,
                         inputBuffer,
                         sizeof(inputBuffer),
                         NULL,
                         (sockaddr *)&clientSocketInfo,
                         &clientSocketInfoLength) == SOCKET_ERROR)
                throw Error::SetErrorMsgText("Receive:",WSAGetLastError());

            cout << "received from Client: " <<  inputBuffer << endl;

            if (sendto(serverSocket,
                       (char *)inputBuffer,
                       sizeof(inputBuffer),
                       NULL,
                       (sockaddr *)&clientSocketInfo,
                       clientSocketInfoLength) == SOCKET_ERROR)
                throw Error::SetErrorMsgText("send:",WSAGetLastError());
        }

//        if (closesocket(serverSocket) == SOCKET_ERROR)
//            throw Error::SetErrorMsgText("CloseSocket: ", WSAGetLastError());
//        if (WSACleanup() == SOCKET_ERROR)
//            throw Error::SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string& errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText << endl;;
    }

    system("pause");
    return 0;
}