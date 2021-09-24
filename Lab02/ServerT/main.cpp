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
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw Error::SetErrorMsgText("OpenSocket:",WSAGetLastError());
        if (bind(serverSocket, (LPSOCKADDR)&serverSocketInfo, sizeof(serverSocketInfo)) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Bind: ", WSAGetLastError());

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Listen: ", WSAGetLastError());

        cout << "server is listening on: "
            << inet_ntoa(serverSocketInfo.sin_addr)
            << ":" << htons(serverSocketInfo.sin_port)
            << endl;

        SOCKET clientSocket;
        SOCKADDR_IN clientSocketInfo;
        int clientSocketInfoLength = 0;

        while (true) {
            clientSocketInfoLength = sizeof(clientSocketInfo);
            memset(&clientSocketInfo, 0, clientSocketInfoLength);

            if ((clientSocket = accept(serverSocket,
                                       (sockaddr*)&clientSocketInfo,
                                       &clientSocketInfoLength)) == INVALID_SOCKET)
                throw  Error::SetErrorMsgText("Accept: ", WSAGetLastError());

            cout << "client connected: "
                 << inet_ntoa(clientSocketInfo.sin_addr)
                 << ":" << htons(clientSocketInfo.sin_port)
                 << endl;

            char inputBuffer[50];

            do {
                if (recv(clientSocket, inputBuffer, sizeof(inputBuffer), NULL) == SOCKET_ERROR)
                    throw Error::SetErrorMsgText("Receive:",WSAGetLastError());

                cout << "received from Client: " <<  inputBuffer << endl;

                if (send(clientSocket, inputBuffer, strlen(inputBuffer) + 1, NULL) == SOCKET_ERROR)
                    throw Error::SetErrorMsgText("send:",WSAGetLastError());
            } while (inputBuffer[0] != '\0');

            cout << "client disconnected: "
                 << inet_ntoa(clientSocketInfo.sin_addr)
                 << ":" << htons(clientSocketInfo.sin_port)
                 << endl;
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