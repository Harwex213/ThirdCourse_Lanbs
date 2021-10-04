#include <Windows.h>
#include <Tlhelp32.h>
#include <iostream>
#include <string>

using namespace std;

int main() {
    DWORD pid = GetCurrentProcessId();
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);

    PROCESSENTRY32 peProcessEntry;
    peProcessEntry.dwSize = sizeof (PROCESSENTRY32);
    
    wcout << L"Current PID = " << pid << endl << L"-----------------------------------" << endl;
    try {
        if (!Process32First(snapshot, &peProcessEntry)) {
            throw L"Process32First";
        }

        do {
            wcout << L"Name = " << peProcessEntry.szExeFile << endl
                  << L"PID = " << peProcessEntry.th32ProcessID
                  << L", Parent PID = " << peProcessEntry.th32ParentProcessID << endl;

            if (peProcessEntry.th32ProcessID == pid) {
                wcout << "--> current process" << endl;
            }

            wcout << L"-----------------------------------" << endl;
        } while (Process32Next(snapshot, &peProcessEntry));
    }
    catch (wchar_t* msg)
    {
        wcout << L"ERROR : " << msg << endl;
    }

    system("pause");
}