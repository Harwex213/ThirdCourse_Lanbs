#include <windows.h>
#include "../include/error/error.h"
#include <iostream>

using namespace std;

int main()
{
    HANDLE handlePipe;
    char pipeName[] = "\\\\.\\pipe\\Tube";

    try {
        cout << "ClientNP started" << endl;

        if(!WaitNamedPipe(pipeName, NMPWAIT_WAIT_FOREVER)) {
            Error::SetPipeError("WaitNamedPipe");
        }

        cout << "ClientNP found specified pipe" << endl;

        if ((handlePipe = CreateFile(pipeName,
                                     GENERIC_READ | GENERIC_WRITE,
                                     0,
                                     nullptr,
                                     OPEN_EXISTING,
                                     0,
                                     nullptr)) == INVALID_HANDLE_VALUE) {
            Error::SetPipeError("CreateFile");
        }

        cout << "ClientNP connected" << endl;

        char readBuffer[256];
        char writeBuffer[256];
        char nextIndexStr[256];
        DWORD readableLength;

        int msgSendCount = 0;
        cout << "Messages count:";
        cin >> msgSendCount;

        DWORD dwMode = PIPE_READMODE_MESSAGE;
        SetNamedPipeHandleState(
                handlePipe,                                      // identifier pipe
                &dwMode,                                    // new mode pipe
                nullptr,                                    // maximum package bytes
                nullptr);                                   // maximum timeout after sent

        for (int i = 0; i <= msgSendCount; ++i) {
            memset(&readBuffer, 0, sizeof(readBuffer));
            if (i != msgSendCount) {
                itoa(i, nextIndexStr, 10);
                strcpy(writeBuffer, "Hello from Client ");
                strcat(writeBuffer, nextIndexStr);
            }
            else {
                strcpy(writeBuffer, "");
            }

            if(!TransactNamedPipe(handlePipe,
                                 writeBuffer,
                                 strlen(writeBuffer) + 1,
                                 readBuffer,
                                 256,
                                 &readableLength,
                                 nullptr)) {
                Error::SetPipeError("TransactNamedPipe");
            }

            cout << "Received from server: " << readBuffer << endl;
        }
    }
    catch (string& errorMsgText) {
        cout << errorMsgText << endl;;
    }

    CloseHandle(handlePipe);

    system("pause");
    return 0;
}