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

        char buffer[256];
        char nextIndexStr[256];
        DWORD readableLength;
        DWORD writableLength;

        int msgSendCount = 0;
        cout << "Messages count:";
        cin >> msgSendCount;

        for (int i = 0; i <= msgSendCount; ++i) {
            if (i != msgSendCount) {
                itoa(i, nextIndexStr, 10);
                strcpy(buffer, "Hello from Client ");
                strcat(buffer, nextIndexStr);
            }
            else {
                strcpy(buffer, "");
            }

            if(!WriteFile(handlePipe, buffer, strlen(buffer) + 1, &writableLength, nullptr)) {
                Error::SetPipeError("WriteFile");
            }

            cout << "Sent: " << buffer << endl;

            if(!ReadFile(handlePipe, buffer, sizeof(buffer), &readableLength, nullptr)) {
                Error::SetPipeError("ReadFile");
            }
        }
    }
    catch (string& errorMsgText) {
        cout << errorMsgText << endl;;
    }

    CloseHandle(handlePipe);

    system("pause");
    return 0;
}