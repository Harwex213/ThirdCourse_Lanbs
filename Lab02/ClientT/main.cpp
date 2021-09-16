#include <iostream>
#include <chrono>
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
        serverSocketInfo.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw Error::SetErrorMsgText("Startup: ", WSAGetLastError());
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw Error::SetErrorMsgText("OpenSocket:",WSAGetLastError());

        cout << "try connect: "
             << inet_ntoa(serverSocketInfo.sin_addr)
             << ":" << htons(serverSocketInfo.sin_port)
             << endl;

        if ((connect(serverSocket, (sockaddr*)&serverSocketInfo, sizeof(serverSocketInfo))) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Connect:", WSAGetLastError());

        char inputBuffer[50];
        char outputBuffer[50];
        char nextIndexStr[5];

        int msgSendCount = 0;
        cout << "messages count:";
        cin >> msgSendCount;

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < msgSendCount; ++i) {
            itoa(i, nextIndexStr, 10);
            strcpy(inputBuffer, "Hello from Client ");
            strcat(inputBuffer, nextIndexStr);

            if (send(serverSocket, inputBuffer, strlen(inputBuffer) + 1, NULL) == SOCKET_ERROR)
                throw  Error::SetErrorMsgText("send:", WSAGetLastError());
            if (recv(serverSocket, outputBuffer, sizeof(outputBuffer), NULL) == SOCKET_ERROR)
                throw  Error::SetErrorMsgText("recv:", WSAGetLastError());
            cout << "received from Server: " << outputBuffer << endl;
        }
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << "Time " << time_span.count() << " seconds." << std::endl << std::endl;

        strcpy(inputBuffer, "");
        if (send(serverSocket, inputBuffer, strlen(inputBuffer) + 1, NULL) == SOCKET_ERROR)
            throw  Error::SetErrorMsgText("send:", WSAGetLastError());
        if (recv(serverSocket, outputBuffer, sizeof(outputBuffer), NULL) == SOCKET_ERROR)
            throw  Error::SetErrorMsgText("recv:", WSAGetLastError());

        if (closesocket(serverSocket) == SOCKET_ERROR)
            throw Error::SetErrorMsgText("CloseSocket: ", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw Error::SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string& errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText << endl;
    }

    system("pause");
}