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
#define MAX_THREADS ( 4 )

DEFINE_GUID(GUID_DEVINTERFACE_HID, 0x4D1E55B2, 0xF16F, 0x11CF, 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30);

typedef struct _DEVICE_DATA {

    BOOL                    HandlesOpen;
    WINUSB_INTERFACE_HANDLE WinusbHandle;
    HANDLE                  DeviceHandle;
    TCHAR                   DevicePath[MAX_PATH];

} DEVICE_DATA, * PDEVICE_DATA;


HRESULT StartGHPoke(_Out_opt_ PBOOL  FailureDeviceNotFound, DWORD   dwThreadIdArray[], HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[]);

HRESULT StopGHPoke(HANDLE  hThreadArray[], PDEVICE_DATA pDeviceData[]);