#include <windows.h>
#include "../include/error/error.h"
#include <iostream>

using namespace std;

int main()
{
    HANDLE handlePipe;
    char pipeName[] = "\\\\.\\pipe\\Tube";

    try {
        cout << "ClientNPct started" << endl;

        if(!WaitNamedPipe(pipeName, NMPWAIT_WAIT_FOREVER)) {
            Error::SetPipeError("WaitNamedPipe");
        }

        cout << "ClientNPct found specified pipe" << endl;

        char readBuffer[256];
        char writeBuffer[256];
        DWORD readableLength;

        strcpy(writeBuffer, "");
        if(!CallNamedPipe(pipeName,
                         writeBuffer,
                         strlen(writeBuffer) + 1,
                         readBuffer,
                         sizeof(readBuffer),
                         &readableLength,
                         1000000)) {
            Error::SetPipeError("CallNamedPipe");
        }
        cout << "Received: " << readBuffer << endl;
    }
    catch (string& errorMsgText) {
        cout << errorMsgText << endl;;
    }

    system("pause");
    return 0;
}