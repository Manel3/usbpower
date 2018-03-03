// IN PROGRESS: test-ioctl-to-hub.c

// Select target Windows version (Macros for Conditional Declarations)
// needed to use e.g.: IOCTL_USB_GET_HUB_CAPABILITIES_EX
// it must be done before include windows.h
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#define NTDDI_VERSION NTDDI_VISTA

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <usbioctl.h>

int main()
{
	// D-LINK HUB
	LPCTSTR lpFileName = "USB\\VID_05E3&PID_0608";
	HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to device 

    hDevice = CreateFile(
		lpFileName,						// name of device to open
        GENERIC_READ | GENERIC_WRITE,	// dwDesiredAccess
        0,								// dwShareMode
        NULL,							// lpSecurityAttributes
        OPEN_EXISTING,					// dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL,			// dwFlagsAndAttributes: Not "| FILE_FLAG_OVERLAPPED" (sync)
        NULL);							// hTemplateFile

    if (hDevice == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open device: %s", lpFileName);
        exit(1);
    }
    else
    {
		printf("Opened device: %s", lpFileName);
	}


	#define FAIL 0

	BOOL bResult   = FALSE;                 // results flag
	DWORD junk     = 0;                     // discard results

	// buffer to store output of this IOCTL
	USB_HUB_CAPABILITIES_EX hub_capabilities;

	// Every IOCTL has its own use of input buffer and output buffer
	bResult = DeviceIoControl(
		hDevice,                           // device to be queried
        IOCTL_USB_GET_HUB_CAPABILITIES_EX, // operation to perform
        NULL, 0,                           // no input buffer
        &hub_capabilities, sizeof(hub_capabilities),   // output buffer
        &junk,                             // # bytes returned
        (LPOVERLAPPED) NULL);              // synchronous I/O

    if( bResult == FAIL )
    {
		printf("Failed DeviceIoControl IOCTL_USB_GET_HUB_CAPABILITIES_EX");
		exit(1);
    }
    else
    {
		printf("DeviceIoControl IOCTL_USB_GET_HUB_CAPABILITIES_EX success");
	}


	#define IOCTL_UNKNOWN 0x88888888

	bResult = DeviceIoControl(
		hDevice,                           // device to be queried
        IOCTL_UNKNOWN,						// operation to perform
        NULL, 0,                           // no input buffer
        NULL, 0,							// no output buffer
        &junk,                             // # bytes returned
        (LPOVERLAPPED) NULL);              // synchronous I/O

    if( bResult == FAIL )
    {
		printf("Failed DeviceIoControl IOCTL_UNKNOWN");
		exit(1);
    }
    else
    {
		printf("DeviceIoControl IOCTL_UNKNOWN");
	}


	CloseHandle(hDevice);
	exit(0);
}
