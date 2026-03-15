#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

DWORD sum;

DWORD WINAPI Summation(LPVOID Param)
{
    DWORD Upper = *(DWORD *)Param;
    sum = 0;
    for (DWORD i = 0; i <= Upper; i++)
    {
        sum += i;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    DWORD ThreadId;
    HANDLE ThreadHandle;
    DWORD Param;

    if (argc != 2)
    {
        fprintf(stderr, "An int parameter is needed\n");
        return -1;
    }

    Param = atoi(argv[1]);
    if ((int)Param < 0)
    {
        fprintf(stderr, "An int must be >= 0\n");
        return -1;
    }

    ThreadHandle = CreateThread(NULL, 0, Summation, &Param, 0, &ThreadId);

    if (ThreadHandle != NULL)
    {
        WaitForSingleObject(ThreadHandle, INFINITE);
        CloseHandle(ThreadHandle);
        printf("Sum = %lu\n", sum);
    }

    return 0;
}
