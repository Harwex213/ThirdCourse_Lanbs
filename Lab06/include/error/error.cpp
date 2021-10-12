#include <string>
#include "Winsock2.h"
using namespace std;

namespace Error
{
    std::string GetPipeError()
    {
        string errorMessage;
        DWORD error = GetLastError();

        switch(error)
        {
            case ERROR_NO_DATA:
                errorMessage = "ERROR_NO_DATA";
                break;
            case ERROR_PIPE_CONNECTED:
                errorMessage = "ERROR_PIPE_CONNECTED";
                break;
            case ERROR_PIPE_LISTENING:
                errorMessage = "ERROR_PIPE_LISTENING";
                break;
            case ERROR_CALL_NOT_IMPLEMENTED:
                errorMessage = "ERROR_CALL_NOT_IMPLEMENTED";
                break;
            case ERROR_INVALID_PARAMETER:
                errorMessage = "ERROR_INVALID_PARAMETER";
                break;
            case ERROR_FILE_NOT_FOUND:
                errorMessage = "ERROR_FILE_NOT_FOUND";
                break;
            default:
                char buffer[256];
                string temp = ulltoa(error, buffer, 10);
                errorMessage = "Error " + temp;
                break;
        }

        return errorMessage;
    }

    std::string SetPipeError(const std::string& source)
    {
        throw source + ": " + GetPipeError();
    }
}