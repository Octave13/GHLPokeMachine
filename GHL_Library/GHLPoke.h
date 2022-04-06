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
#define PS3_WIIU_SLEEP_TIME     ( 8 * ONE_SECOND ) // 8 seconds

#define PS4_VID_PID             ( L"vid_1430&pid_07bb" )
#define PS4_POKE_MESSAGE        { 0x30, 0x02, 0x08, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define PS4_SLEEP_TIME          ( 6 * ONE_SECOND ) // 6 seconds

//#define XBOXONE_VID_PID             ( L"vid_1430&pid_079b" )
#define XBOXONE_VID_PID             ( L"vid_045e&pid_02ff" )
#define XBOXONE_POKE_MESSAGE        { 0x20,0x20, 0x22,0x00,0x00, 0x08, 0x02, 0x08, 0x20  } /*0x4b*/
//0x22,0x00,0x00, 0x08, 0x02, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 
#define XBOXONE_SLEEP_TIME          ( 16 * ONE_SECOND ) // 16 seconds

DEFINE_GUID(GUID_DEVINTERFACE_HID, 0x4D1E55B2, 0xF16F, 0x11CF, 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30);
DEFINE_GUID(GUID_DEVINTERFACE_XBOX, 0x05F5CFE2, 0x4733, 0x4950, 0xA6, 0xBB, 0x07, 0xAA, 0xD0, 0x1A, 0x3A, 0x84);
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);

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