#include "Service.h"
#include "sddl.h"

int main(int argc, char* argv[])
{
    time_t currentTime = time(NULL);
    char currentTimeString[32];
    ctime_s(currentTimeString, 32, &currentTime);
    currentTimeString[24] = '\0';
    trace("----------------------------------------");
    trace("ServiceMain called. ", currentTimeString);

    if (argc < 4) {
        trace("ServiceMain start failed. ", "Invalid arguments amount. ",
            "Should be storagePath(char*), snapshotsDirectoryPath(char*), loggerPath(char*)");
        return -1;
    }

    storagePath = argv[1];
    snapshotsDirectoryPath = argv[2];
    loggerPath = argv[3];
    trace("Storage path: ", storagePath);
    trace("Snapshots directory path: ", snapshotsDirectoryPath);
    trace("Logger path: ", loggerPath.c_str());

    SERVICE_TABLE_ENTRY serviceStartTable[] = {
        { ServiceName, ServiceMain },
        { NULL, NULL }
    };
    if (!StartServiceCtrlDispatcher(serviceStartTable))
    {
        trace("ServiceMain::StartServiceCtrlDispatcher: failed. GetLastError() - ", GetLastError());
    }
}
