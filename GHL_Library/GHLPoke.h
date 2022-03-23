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

#define PS3_WIIU_VID_PID        ( L"vid_12ba&pid_074b" )
#define PS3_WIIU_POKE_MESSAGE   { 0x02, 0x02, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define PS4_VID_PID             ( L"vid_1430&pid_07bb" )
#define PS4_POKE_MESSAGE        { 0x30, 0x02, 0x08, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define POKE_MESSAGE_LENGTH     ( 9 )
#define MAX_THREADS ( 4 )

typedef enum  MachineType { PS3_WIIU, PS4 };


DEFINE_GUID(GUID_DEVINTERFACE_HID, 0x4D1E55B2, 0xF16F, 0x11CF, 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30);

typedef struct _DEVICE_DATA 
{

    BOOL                    HandlesOpen;
    WINUSB_INTERFACE_HANDLE WinusbHandle;
    HANDLE                  DeviceHandle;
    TCHAR                   DevicePath[MAX_PATH];
    MachineType             DeviceType;

} DEVICE_DATA, * PDEVICE_DATA;


HRESULT StartGHPoke(_Out_opt_ PBOOL  FailureDeviceNotFound, DWORD   dwThreadIdArray[], HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[]);

HRESULT StopGHPoke(HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[]);