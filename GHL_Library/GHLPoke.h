#pragma once

//
// Define below GUIDs
//


/*
#include <initguid.h>
#include <cfgmgr32.h>
#include <setupapi.h>
#include <winioctl.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <winusb.h>
#include <usb.h>
*/
#define ONE_SECOND              ( 1000 ) // One second
#define POKE_MESSAGE_LENGTH     ( 9 )
#define MAX_THREADS ( 4 )

#define PS3_WIIU_VID_PID        ( L"vid_12ba&pid_074b" )
#define PS3_WIIU_POKE_MESSAGE   { 0x02, 0x02, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define PS3_WIIU_SLEEP_TIME     ( 10 * ONE_SECOND ) // 10 seconds

#define PS4_VID_PID             ( L"vid_1430&pid_07bb" )
#define PS4_POKE_MESSAGE        { 0x30, 0x02, 0x08, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define PS4_SLEEP_TIME          ( 8 * ONE_SECOND ) // 8 seconds

DEFINE_GUID(GUID_DEVINTERFACE_HID, 0x4D1E55B2, 0xF16F, 0x11CF, 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30);

typedef enum DeviceType { UNKNOWN_DEVICE = 0, PS3_WIIU, PS4, XBOXONE };

typedef struct _DEVICE_DATA 
{

    BOOL                    HandlesOpen;
    HANDLE                  DeviceHandle;
    WINUSB_INTERFACE_HANDLE InterfaceHandle;
    int                     SleepTime;
    CWnd                    *DlgItem;
    TCHAR                   DevicePath[MAX_PATH];
    CHAR                    PokeMessage[POKE_MESSAGE_LENGTH];

} DEVICE_DATA, * PDEVICE_DATA;


HRESULT StartGHPoke(_Out_opt_ PBOOL  FailureDeviceNotFound, PDWORD   *dwThreadIdArray, PHANDLE  *hThreadArray, PDEVICE_DATA *pDeviceData, int* pNbRemote);

HRESULT StopGHPoke(PDWORD   *dwThreadIdArray, PHANDLE  *hThreadArray, PDEVICE_DATA *pDeviceData);

BOOL SetStaticText(CWnd* Text, LPCTSTR lpszString);

int GetItemNb(int i);

VOID SetStartThread(BOOL Bool);