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

        SOCKADDR_IN serverSocketInfo;
        if (EstablishConnection(2000, (char *) "Hello", serverSocketInfo)) {
            cout << "Was established connection with server "
                 << inet_ntoa(serverSocketInfo.sin_addr)
                 << ":"
                 << htons(serverSocketInfo.sin_port)
                 << endl;
        }
    }
    catch (string& errorMsgText) {
        cout << endl << "WSAGetLastError: " << errorMsgText << endl;;
    }
    system("pause");
}