#include <windows.h>
#include "../include/error/error.h"
#include <iostream>

using namespace std;

int main()
{
    HANDLE handlePipe;
    char pipeName[] = "\\\\.\\pipe\\Tube";

    char buffer[256];
    DWORD readableLength;
    DWORD writableLength;

    cout << "ServerNP started" << endl;

    while(true) {
        try {
            if ((handlePipe = CreateNamedPipe(pipeName,
                                              PIPE_ACCESS_DUPLEX,
                                              PIPE_TYPE_MESSAGE | PIPE_WAIT,
                                              1,
                                              0,
                                              0,
                                              INFINITE,
                                              nullptr)) == INVALID_HANDLE_VALUE) {
                Error::SetPipeError("CreateNamedPipe");
            }

            cout << "Named Pipe was created" << endl;

            if(!ConnectNamedPipe(handlePipe, nullptr)) {
                Error::SetPipeError("ConnectNamedPipe");
            }

            cout << "Pipe was connected" << endl;

            do {
                if(!ReadFile(handlePipe, buffer, sizeof(buffer), &readableLength, nullptr)) {
                    Error::SetPipeError("ReadFile");
                }

                cout << "Received: " << buffer << endl;

                if(!WriteFile(handlePipe, buffer, strlen(buffer) + 1, &writableLength, nullptr)) {
                    Error::SetPipeError("WriteFile");
                }
            } while(buffer[0] != '\0');
        }
        catch (string& errorMsgText) {
            cout << errorMsgText << endl;;
        }

        CloseHandle(handlePipe);
    }
}