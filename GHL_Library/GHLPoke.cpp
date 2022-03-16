#include "../pch.h"
#include "pch.h"
#include "GHLPoke.h"



BOOL Continue = TRUE;

int j = 0;

DWORD WINAPI PS4Poke(LPVOID lpParam);

DWORD WINAPI PS3WiiUPoke(LPVOID lpParam);

HRESULT StartGHPoke(_Out_opt_ PBOOL  FailureDeviceNotFound, DWORD   dwThreadIdArray[], HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[])
{

    BOOL                             bResult = FALSE;
    HDEVINFO                         deviceInfo;
    SP_DEVICE_INTERFACE_DATA         interfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = NULL;
    ULONG                            length;
    ULONG                            requiredLength = 0;
    HRESULT                          hr = NO_ERROR;
    int i = 0;
    if (NULL != FailureDeviceNotFound) {

        *FailureDeviceNotFound = FALSE;
    }

    //
    // Enumerate all devices exposing the interface
    //
    deviceInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_HID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if (deviceInfo == INVALID_HANDLE_VALUE) {

        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    while (Continue)
    {
        //
        // Get the next interface (index i) in the result set
        //
        bResult = SetupDiEnumDeviceInterfaces(deviceInfo, NULL, &GUID_DEVINTERFACE_HID, i, &interfaceData);

        if (FALSE == bResult)
        {

            //
            // We would see this error if no devices were found
            //
            if (ERROR_NO_MORE_ITEMS == GetLastError() && NULL != FailureDeviceNotFound)
            {

                if (j == 0)
                {
                    *FailureDeviceNotFound = TRUE;
                }
            }

            hr = HRESULT_FROM_WIN32(GetLastError());
            SetupDiDestroyDeviceInfoList(deviceInfo);
            return hr;
        }

        //
        // Get the size of the path string
        // We expect to get a failure with insufficient buffer
        //
        bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, NULL, 0, &requiredLength, NULL);

        if (FALSE == bResult && ERROR_INSUFFICIENT_BUFFER != GetLastError())
        {

            hr = HRESULT_FROM_WIN32(GetLastError());
            SetupDiDestroyDeviceInfoList(deviceInfo);
            return hr;
        }
        //
        // Allocate temporary space for SetupDi structure
        //
        detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, requiredLength);

        if (NULL == detailData)
        {
            hr = E_OUTOFMEMORY;
            SetupDiDestroyDeviceInfoList(deviceInfo);
            return hr;
        }

        detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        length = requiredLength;

        //
        // Get the interface's path string
        //
        bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, detailData, length, &requiredLength, NULL);

        if (FALSE == bResult)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            LocalFree(detailData);
            SetupDiDestroyDeviceInfoList(deviceInfo);
            return hr;
        }

        if (wcsstr(detailData->DevicePath, L"vid_12ba&pid_074b"))
        {
            pDeviceData[j] = (PDEVICE_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DEVICE_DATA));
            if (pDeviceData == NULL)
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                LocalFree(detailData);
                SetupDiDestroyDeviceInfoList(deviceInfo);
                return hr;
            }

            StringCbCopy(pDeviceData[j]->DevicePath, MAX_PATH - 1, detailData->DevicePath);

            hThreadArray[j] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size  
                PS3WiiUPoke,       // thread function name
                pDeviceData[j], // argument to thread function 
                0,                      // use default creation flags 
                &dwThreadIdArray[j]);   // returns the thread identifier 

            if (hThreadArray[j] == NULL)
            {
                LocalFree(detailData);
                SetupDiDestroyDeviceInfoList(deviceInfo);
                ExitProcess(3);
            }

            j++;

        }
        else if (wcsstr(detailData->DevicePath, L"vid_1430&pid_07bb"))
        {
            pDeviceData[j] = (PDEVICE_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DEVICE_DATA));
            if(pDeviceData == NULL)
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                LocalFree(detailData);
                SetupDiDestroyDeviceInfoList(deviceInfo);
                return hr;
            }

            StringCbCopy(pDeviceData[j]->DevicePath, MAX_PATH - 1, detailData->DevicePath);
            hThreadArray[j] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size  
                PS4Poke,       // thread function name
                pDeviceData[j], // argument to thread function 
                0,                      // use default creation flags 
                &dwThreadIdArray[j]);   // returns the thread identifier 

            if (hThreadArray[j] == NULL)
            {
                LocalFree(detailData);
                SetupDiDestroyDeviceInfoList(deviceInfo);
                ExitProcess(3);
            }

            j++;

        }
        else
        {
            LocalFree(detailData);
        }
        i++;
    }


    LocalFree(detailData);
    SetupDiDestroyDeviceInfoList(deviceInfo);

    return hr;
}


#define BUFSIZE 9

DWORD WINAPI PS3WiiUPoke(LPVOID lpParam)
{
    HRESULT hr = S_OK;
    PDEVICE_DATA DeviceData = (PDEVICE_DATA)lpParam;
    BOOL    bResult;
    CHAR Buffer[BUFSIZE];

    DeviceData->DeviceHandle = CreateFile(DeviceData->DevicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

    if (INVALID_HANDLE_VALUE == DeviceData->DeviceHandle)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    Buffer[0] = 0x02;//a cause des conventions GAB!
    Buffer[1] = 0x02;
    Buffer[2] = 0x08;
    Buffer[3] = 0x20;
    Buffer[4] = 0x00;
    Buffer[5] = 0x00;
    Buffer[6] = 0x00;
    Buffer[7] = 0x00;
    Buffer[8] = 0x00;

    while (Continue)
    {
        bResult = HidD_SetOutputReport(DeviceData->DeviceHandle, Buffer, BUFSIZE * sizeof(CHAR));
        if (!bResult)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            CloseHandle(DeviceData->DeviceHandle);
            return hr;
        }
        Sleep(6000);
    }

    WinUsb_Free(DeviceData->WinusbHandle);
    CloseHandle(DeviceData->DeviceHandle);
    DeviceData->HandlesOpen = FALSE;

    return hr;
}


DWORD WINAPI PS4Poke(LPVOID lpParam)
{
    HRESULT hr = S_OK;
    PDEVICE_DATA DeviceData = (PDEVICE_DATA)lpParam;
    BOOL    bResult;
    CHAR Buffer[BUFSIZE];

    DeviceData->DeviceHandle = CreateFile(DeviceData->DevicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

    if (INVALID_HANDLE_VALUE == DeviceData->DeviceHandle)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    Buffer[0] = 0x30;//Les conventions GAB?
    Buffer[1] = 0x02;
    Buffer[2] = 0x08;
    Buffer[3] = 0x0A;
    Buffer[4] = 0x00;
    Buffer[5] = 0x00;
    Buffer[6] = 0x00;
    Buffer[7] = 0x00;
    Buffer[8] = 0x00;

    while (Continue)
    {
        bResult = HidD_SetOutputReport(DeviceData->DeviceHandle, Buffer, BUFSIZE * sizeof(CHAR));
        if (!bResult)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            CloseHandle(DeviceData->DeviceHandle);
            return hr;
        }
        Sleep(6000);
    }
    WinUsb_Free(DeviceData->WinusbHandle);
    CloseHandle(DeviceData->DeviceHandle);
    DeviceData->HandlesOpen = FALSE;
    return hr;
}


HRESULT StopGHPoke(HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[])
{
    Continue = FALSE;

    WaitForMultipleObjects(j, hThreadArray, TRUE, INFINITE);

    for (int i = 0; i < j; i++)
    {
        CloseHandle(hThreadArray[i]);
        if (pDeviceData[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pDeviceData[i]);
            pDeviceData[i] = NULL;    // Ensure address is not reused.
        }
    }

    return 0;
}