/************************************************ Include ******************************************/
#include "../pch.h"
#include "pch.h"
#include "GHLPoke.h"


/**************************************** global variable ******************************************/
BOOL Continue = TRUE;
int j = 0;
const CHAR Ps3WiiuPokeMessage[POKE_MESSAGE_LENGTH] = PS3_WIIU_POKE_MESSAGE;
const CHAR Ps4PokeMessage[POKE_MESSAGE_LENGTH] = PS4_POKE_MESSAGE;

/************************************* private function prototype***********************************/


DeviceType CheckVidPid(WCHAR* DevicePath);

BOOL InitializeData(PDEVICE_DATA pDeviceData, WCHAR* DevicePath, DeviceType Device);

DWORD WINAPI SendPokeMessage(LPVOID lpParam);

/********************************** Function *******************************************************/

HRESULT StartGHPoke(_Out_opt_ PBOOL  FailureDeviceNotFound, DWORD   dwThreadIdArray[], HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[])
{

    BOOL                             bResult = FALSE;
    HDEVINFO                         deviceInfo;
    SP_DEVICE_INTERFACE_DATA         interfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = NULL;
    ULONG                            length;
    ULONG                            requiredLength = 0;
    HRESULT                          hr = NO_ERROR;
    DeviceType                       Device = UNKNOWN_DEVICE;
    int i = 0;


    if (NULL != FailureDeviceNotFound) 
    {
        *FailureDeviceNotFound = FALSE;
    }

    // Enumerate all devices exposing the HID interface
    deviceInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_HID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (deviceInfo == INVALID_HANDLE_VALUE) 
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }
    interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);


    while (Continue)
    {
        // Get the next interface (index i) in the result set
        bResult = SetupDiEnumDeviceInterfaces(deviceInfo, NULL, &GUID_DEVINTERFACE_HID, i, &interfaceData);
        if (FALSE == bResult)
        {
            // We would see this error if no devices were found
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

        // Get the size of the path string
        // We expect to get a failure with insufficient buffer
        bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, NULL, 0, &requiredLength, NULL);
        if (FALSE == bResult && ERROR_INSUFFICIENT_BUFFER != GetLastError())
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            SetupDiDestroyDeviceInfoList(deviceInfo);
            return hr;
        }

        // Allocate temporary space for SetupDi structure
        detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, requiredLength);
        if (NULL == detailData)
        {
            hr = E_OUTOFMEMORY;
            SetupDiDestroyDeviceInfoList(deviceInfo);
            return hr;
        }
        detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        length = requiredLength;

        // Get the interface's path string
        bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo, &interfaceData, detailData, length, &requiredLength, NULL);
        if (FALSE == bResult)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            LocalFree(detailData);
            SetupDiDestroyDeviceInfoList(deviceInfo);
            return hr;
        }

        /* Check if The device is one from the list */
        if (Device = CheckVidPid(detailData->DevicePath))
        {
            /* Memory Allocation for data Pointer */
            if (j == 0)
            {

            }
            else
            {
                //realloc(pDeviceData, (j + 1) * sizeof(DEVICE_DATA));
            }

            /* Memory Allocation for Data */

            pDeviceData[j] = (PDEVICE_DATA)malloc(sizeof(DEVICE_DATA) );


            /* Initialize Data */
            if (InitializeData(pDeviceData[j], detailData->DevicePath, Device))
            {

                /* Memory Allocation for data Pointer */
                if (j == 0)
                {

                    //hThreadArray = (PHANDLE)malloc(sizeof(HANDLE));
                }
                else
                {

                    hThreadArray = (PHANDLE)realloc(hThreadArray, (j + 1) * sizeof(HANDLE));
                }

                /* Memory Allocation for data Pointer */
                if (j == 0)
                {

                    //dwThreadIdArray = (PDWORD)malloc(sizeof(DWORD));
                }
                else
                {

                    dwThreadIdArray = (PDWORD)realloc(dwThreadIdArray, (j + 1) * sizeof(DWORD));
                }

                hThreadArray[j] = CreateThread(
                    NULL,                   // default security attributes
                    0,                      // use default stack size  
                    SendPokeMessage,        // thread function name
                    pDeviceData[j],         // argument to thread function 
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

        }

        /* Increment Device Interface Index*/
        i++;
    }

    LocalFree(detailData);
    SetupDiDestroyDeviceInfoList(deviceInfo);

    return hr;
}

DeviceType CheckVidPid(WCHAR *DevicePath)
{
    if (wcsstr(DevicePath, PS4_VID_PID))
        return PS4;
    else if(wcsstr(DevicePath, PS3_WIIU_VID_PID))
        return PS3_WIIU;
    else
        /* Not a recognized Device*/
        return UNKNOWN_DEVICE;
}


BOOL InitializeData(PDEVICE_DATA pDeviceData, WCHAR* DevicePath, DeviceType Device)
{
    /* Copy Device Path in device Data */
    if (StringCbCopy(pDeviceData->DevicePath, MAX_PATH - 1, DevicePath) != S_OK)
    {
        pDeviceData = NULL;
        return FALSE;
    }
    /* Open File */
    pDeviceData->DeviceHandle = CreateFile(pDeviceData->DevicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (INVALID_HANDLE_VALUE == pDeviceData->DeviceHandle)
    {
        pDeviceData = NULL;
        return FALSE;
    }

    /* Flag for handle open */
    pDeviceData->HandlesOpen = TRUE;

    /* Initialize Specific Device Data */
    switch (Device)
    {
    case PS4: 
        memcpy_s(pDeviceData->PokeMessage, POKE_MESSAGE_LENGTH, Ps4PokeMessage, POKE_MESSAGE_LENGTH);
        pDeviceData->SleepTime = PS4_SLEEP_TIME;
        break;
    case PS3_WIIU:
        memcpy_s(pDeviceData->PokeMessage, POKE_MESSAGE_LENGTH, Ps3WiiuPokeMessage, POKE_MESSAGE_LENGTH);
        pDeviceData->SleepTime = PS3_WIIU_SLEEP_TIME;
        break;
    default: 
        /* Not supported */
        break;
    }

    /* Send a first Poke. Some device have more than one interface */
    if (!HidD_SetOutputReport(pDeviceData->DeviceHandle, pDeviceData->PokeMessage, POKE_MESSAGE_LENGTH * sizeof(CHAR)))
    {

        CloseHandle(pDeviceData->DeviceHandle);
        //WinUsb_Free(pDeviceData->WinusbHandle);
        pDeviceData->HandlesOpen = FALSE;
        pDeviceData = NULL;
        return FALSE;
    }

    return TRUE;

}

DWORD WINAPI SendPokeMessage(LPVOID lpParam)
{
    HRESULT hr = S_OK;
    PDEVICE_DATA DeviceData = (PDEVICE_DATA)lpParam;
    BOOL    bResult;

    while (Continue)
    {
        /* Send Poke */
        bResult = HidD_SetOutputReport(DeviceData->DeviceHandle, DeviceData->PokeMessage, POKE_MESSAGE_LENGTH * sizeof(CHAR));
        if (!bResult)
        {
            /* If error eg: Deconnexion */
            hr = HRESULT_FROM_WIN32(GetLastError());
            CloseHandle(DeviceData->DeviceHandle);
            DeviceData->HandlesOpen = FALSE;
            return hr;
        }
        /* Wait for next poke */
        Sleep(DeviceData->SleepTime);
    }

    CloseHandle(DeviceData->DeviceHandle);
    DeviceData->HandlesOpen = FALSE;
    //free(DeviceData);
    //DeviceData = NULL;
    return hr;
}


HRESULT StopGHPoke(DWORD   dwThreadIdArray[], HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[] )
{
    Continue = FALSE;

    WaitForMultipleObjects(j, hThreadArray, TRUE, INFINITE);

    for (int i = j-1;  0 <= i; i--)
    {
        CloseHandle(hThreadArray);
        //free(pDeviceData[i]);
    }
   
    if (dwThreadIdArray != NULL)
    {
        free(dwThreadIdArray);
        dwThreadIdArray = NULL;
    }
    if (hThreadArray != NULL)
    {
        free(hThreadArray);
        hThreadArray = NULL;
    }
    /*
    if (pDeviceData != NULL)
    {
        free(pDeviceData);
        pDeviceData = NULL;
    }
    */
    Continue = TRUE;
    j = 0;

    return 0;
}