// TODO: Use unicode and wprintf instead of printf, now ASCII

// Select target Windows version (Macros for Conditional Declarations)
// needed to use e.g.: IOCTL_USB_GET_HUB_CAPABILITIES_EX
// it must be done before include windows.h
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#define NTDDI_VERSION NTDDI_VISTA

// UNICODE support in standard C functions
//#define _UNICODE
//#include <tchar.h>
//#include <wchar.h>

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <usbioctl.h>


// Modified from MS example: "Retrieving the Last-Error Code"
void PrintError(char *lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    char *lpMsgBuf;		//FormatMessage will allocate the buffer
    DWORD dw = GetLastError(); 

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        0,					//dwLanguageId auto
        (char *) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    printf("Error: %s failed with error %lx: %s \n",
		lpszFunction, dw, lpMsgBuf);
 
    LocalFree(lpMsgBuf);
}


int main(int argc, char *argv[])
{
	// show help on less or more than 1 parameter, prog /ANY-FLAG, prog help or prog HELP
	// program name + 1 argv
	if( argc != 1 + 1 || strncmp( argv[1], "/", 1)==0 || strcmp( argv[1], "help" )==0 || strcmp( argv[1], "HELP" )==0 ){
		printf( "test-ioctl-to-hub version 0.02  (c)2017 Manel Marin \n\n");
		printf( "Usage: test-ioctl-to-hub ExternalDeviceName \n");
		printf( "To find ExternalDeviceName use WinObj.exe from MS \n\n");
		printf( "Example D-LINK H4 in my PC: \n");
		printf( "  test-ioctl-to-hub \"\\\\?\\USB#VID_05E3&PID_0608#5&147EFEE4&0&3#{f18a0e88-c30c-11d0-8815-00a0c906bed8}\" \n" );
		exit(0);
	}
	char *DeviceName = argv[1];


	// D-LINK HUB
	LPTSTR lpFileName = DeviceName;
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
        printf("Failed to open device: %s \n", lpFileName);
        PrintError("CreateFile");
    }
    else
    {
		printf("Opened device: %s \n", lpFileName);
	}


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

    if( bResult == FALSE )
    {
		printf("Failed DeviceIoControl IOCTL_USB_GET_HUB_CAPABILITIES_EX \n");
        PrintError("DeviceIoControl");
    }
    else
    {
		printf("DeviceIoControl IOCTL_USB_GET_HUB_CAPABILITIES_EX success \n");
	}


	#define IOCTL_UNKNOWN 0x88888888
	bResult   = FALSE;                 // results flag
	junk = 0;

	bResult = DeviceIoControl(
		hDevice,                           // device to be queried
        IOCTL_UNKNOWN,						// operation to perform
        NULL, 0,                           // no input buffer
        NULL, 0,							// no output buffer
        &junk,                             // # bytes returned
        (LPOVERLAPPED) NULL);              // synchronous I/O

    if( bResult == FALSE )
    {
		printf("Failed DeviceIoControl IOCTL_UNKNOWN \n");
        PrintError("DeviceIoControl");
    }
    else
    {
		printf("DeviceIoControl IOCTL_UNKNOWN success \n");
	}


	CloseHandle(hDevice);
	exit(0);
}
