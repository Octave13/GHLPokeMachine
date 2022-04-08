/************************************************ Include ******************************************/
#include "../pch.h"
#include "pch.h"
#include "GHLPoke.h"


/**************************************** global variable ******************************************/
BOOL Start = FALSE;
BOOL Continue = TRUE;
int j = 0;
const CHAR Ps3WiiuPokeMessage[POKE_MESSAGE_LENGTH] = PS3_WIIU_POKE_MESSAGE;
const CHAR Ps4PokeMessage[POKE_MESSAGE_LENGTH] = PS4_POKE_MESSAGE;

/************************************* private function prototype***********************************/


DeviceType CheckVidPid(WCHAR* DevicePath);

BOOL InitializeData(PDEVICE_DATA pDeviceData, WCHAR* DevicePath, DeviceType Device);

DWORD WINAPI SendPokeMessage(LPVOID lpParam);

LPCTSTR GetDeviceString(PDEVICE_DATA DeviceData);

/********************************** Function *******************************************************/

HRESULT StartGHPoke(_Out_opt_ PBOOL  FailureDeviceNotFound, PDWORD   *dwThreadIdArray, PHANDLE  *hThreadArray, PDEVICE_DATA *pDeviceData, int *pNbRemote)
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
    deviceInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_HID, NULL, NULL, DIGCF_PRESENT  | DIGCF_DEVICEINTERFACE);
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
                else
                {
                    for (int i = 0; i < j; i++)
                    {
                        (*hThreadArray)[i] = CreateThread(
                            NULL,                   // default security attributes
                            0,                      // use default stack size  
                            SendPokeMessage,        // thread function name
                            &(*pDeviceData)[i],     // argument to thread function 
                            0,                      // use default creation flags 
                            &(*dwThreadIdArray)[i]);   // returns the thread identifier 
                        if ((*hThreadArray)[i] == NULL)
                        {
                            LocalFree(detailData);
                            SetupDiDestroyDeviceInfoList(deviceInfo);
                            ExitProcess(3);
                        }
                    }
                    *FailureDeviceNotFound = FALSE;
                    *pNbRemote = j;
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
            *pDeviceData = (PDEVICE_DATA)realloc(*pDeviceData, (j + 1) * sizeof(DEVICE_DATA));
            if (*pDeviceData == NULL)
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                LocalFree(detailData);
                SetupDiDestroyDeviceInfoList(deviceInfo);
                return hr;
            }

            /* Initialize Data */
            if (InitializeData(&(*pDeviceData)[j], detailData->DevicePath, Device))
            {

                /* Memory Allocation for data Pointer */
                 *hThreadArray = (PHANDLE)realloc(*hThreadArray, (j + 1) * sizeof(HANDLE));
                 if (*hThreadArray == NULL)
                 {
                     hr = HRESULT_FROM_WIN32(GetLastError());
                     LocalFree(detailData);
                     SetupDiDestroyDeviceInfoList(deviceInfo);
                     return hr;
                 }

                /* Memory Allocation for data Pointer */
                *dwThreadIdArray = (PDWORD)realloc(*dwThreadIdArray, (j + 1) * sizeof(DWORD));
                if (*dwThreadIdArray == NULL)
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    LocalFree(detailData);
                    SetupDiDestroyDeviceInfoList(deviceInfo);
                    return hr;
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
    else if (wcsstr(DevicePath, PS3_WIIU_VID_PID))
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
    };

    /* Send a first Poke. Some device have more than one interface */
    if (!HidD_SetOutputReport(pDeviceData->DeviceHandle, pDeviceData->PokeMessage, POKE_MESSAGE_LENGTH * sizeof(CHAR)))
    {
        CloseHandle(pDeviceData->DeviceHandle);
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
    int i = 0;

    while ( !Start || !Continue)
    {
        Sleep(ONE_SECOND);
    }

    SetStaticText(DeviceData->DlgItem, GetDeviceString(DeviceData));

    while (Continue)
    {
        /* Send Poke */
        bResult = HidD_SetOutputReport(DeviceData->DeviceHandle, DeviceData->PokeMessage, POKE_MESSAGE_LENGTH * sizeof(CHAR));
        if (!bResult)
        {
            /* If error eg: Deconnexion */
            hr = GetLastError();
            
            MessageBox(NULL, (LPCWSTR)L"One device was disconnected\nCheck the connection, press stop and then start",
                (LPCWSTR)GetDeviceString(DeviceData),
                MB_ICONEXCLAMATION | MB_OK | MB_DEFBUTTON1
            );
            SetStaticText(DeviceData->DlgItem, L"Connexion Problem");
            CloseHandle(DeviceData->DeviceHandle);
            DeviceData->HandlesOpen = FALSE;
            return hr;
        }

        /* Wait for next poke */
        Sleep(DeviceData->SleepTime);
    }
    CloseHandle(DeviceData->DeviceHandle);
    DeviceData->HandlesOpen = FALSE;
    return hr;
}


HRESULT StopGHPoke(PDWORD   *dwThreadIdArray, PHANDLE  *hThreadArray, PDEVICE_DATA *pDeviceData )
{
    Continue =  FALSE;

    WaitForMultipleObjects(j, *hThreadArray, TRUE, INFINITE);

    if (*dwThreadIdArray != NULL)
    {
        free(*dwThreadIdArray);
        *dwThreadIdArray = NULL;
    }
    if (*hThreadArray != NULL)
    {
        free(*hThreadArray);
        *hThreadArray = NULL;
    }
  
    if (*pDeviceData != NULL)
    {
        free(*pDeviceData);
        *pDeviceData = NULL;
    }

    SetStartThread(FALSE);
    Continue = TRUE;

    j = 0;

    return 0;
}

#define IDC_STATIC2 1006
#define IDC_STATIC3 1007
#define IDC_STATIC4 1008

int GetItemNb(int i)
{
    switch (i)
    {
        case 0:
            return IDC_STATIC;
        case 1:
            return IDC_STATIC2;
        case 2:
            return IDC_STATIC3;
        case 3:
            return IDC_STATIC4;
        default:
            return IDC_STATIC;
        
    }
}

LPCTSTR GetDeviceString(PDEVICE_DATA DeviceData)
{
    if (wcsstr(DeviceData->DevicePath, PS4_VID_PID))
        return L"PS4 Guitar";
    else if (wcsstr(DeviceData->DevicePath, PS3_WIIU_VID_PID))
        return L"PS3/WIIU Guitar";
    else
        /* Not a recognized Device*/
        return L"Unknown Device";
}

BOOL SetStaticText(CWnd* Text, LPCTSTR lpszString)
{
    if (Text != NULL)
    {
        Text->SetWindowTextW(lpszString);
        return TRUE;
    }
    return FALSE;
}

VOID SetStartThread(BOOL Bool)
{
    if (Bool)
    {
        Start = TRUE;
    }
    else
    {
        Start = FALSE;
    }
}