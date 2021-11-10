#include <iostream>
#include <windows.h>
using namespace std;

void sh(HANDLE pheap) {
    PROCESS_HEAP_ENTRY proc_heap_entry;
    proc_heap_entry.lpData = nullptr;

    int heapSize = 0;
    int uncommittedSize = 0;
    int busySize = 0;
    int othersSize = 0;

    while (HeapWalk(pheap, &proc_heap_entry)) {

        if (proc_heap_entry.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) 
        {
            uncommittedSize += proc_heap_entry.cbData;
        }
        else if (proc_heap_entry.wFlags & PROCESS_HEAP_ENTRY_BUSY)
        {
            busySize += proc_heap_entry.cbData;
        }
        else
        {
            othersSize += proc_heap_entry.cbData;
        }

        heapSize += proc_heap_entry.cbData;
    }

    cout << "heap size: " << heapSize << endl;
    cout << "uncommited size: " << uncommittedSize << endl;
    cout << "busy size: " << busySize << endl;
    cout << "others size: " << othersSize << endl;
    cout << endl;
}

int main()
{
    HANDLE pheap = GetProcessHeap();

    sh(pheap);
    int* array = new int[300000];
    sh(pheap);

    system("pause");
}
