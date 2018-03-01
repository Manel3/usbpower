## Howto find handle for an IOCTL

DeviceIoControl need a handle (named hDevice) to determine the device, how to get it?.

API doc tells to use the CreateFile function to retrieve it, we will need a device name string to call it.

## Key Windows API definitions and calls:
- call IOCTL: [DeviceIoControl](https://msdn.microsoft.com/es-es/library/windows/desktop/aa363216(v=vs.85).aspx)
    - [Calling DeviceIoControl](https://msdn.microsoft.com/es-es/library/windows/desktop/aa363147(v=vs.85).aspx)
- get handle: CreateFile
    - [Naming Files, Paths, and Namespaces](https://msdn.microsoft.com/es-es/library/windows/desktop/aa365247(v=vs.85).aspx)
    - USB devices names: "USB\\VID_XXXX&PID_XXXX" (TODO: TEST THAT)
- existing USB IOCTL codes: [usbioctl.h](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbioctl/)
    - Note: IOCTL with **_INTERNAL_** on them can only be called from a driver, and not from user space

## TESTING IT:
Lets try to comunicate with USB HUB with existing IOCTL:
- [IOCTL_USB_GET_HUB_CAPABILITIES_EX](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbioctl/ni-usbioctl-ioctl_usb_get_hub_capabilities_ex)
- And also with a not existing IOCTL to test the error return

TODO

### How it is done on:
In fact usbdk project uses CreateFile on UsbDk-1.00-19/UsbDkHelper/DriverFile.cpp

``` c
UsbDkDriverFile::UsbDkDriverFile(LPCTSTR lpFileName, bool bOverlapped)
{
    m_bOverlapped = bOverlapped;

    m_hDriver = CreateFile(lpFileName,
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           nullptr,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL | (bOverlapped ? FILE_FLAG_OVERLAPPED : 0),
                           nullptr);

    if (m_hDriver == INVALID_HANDLE_VALUE)
    {
        throw UsbDkDriverFileException(tstring(TEXT("Failed to open device symlink ")) + lpFileName);
    }
}
```
Lets see how must be created **lpFileName** 

In usbdk project it comes from: 
- UsbDkDriverFile(LPCTSTR lpFileName, bool bOverlapped = false);
- class UsbDkRedirectorAccess : public UsbDkDriverFile (in RedirectorAccess.h)
- (indirectly from:) HANDLE UsbDk_StartRedirect(PUSB_DK_DEVICE_ID DeviceID)
- static int Controller_RedirectDevice(TCHAR *DeviceID, TCHAR *InstanceID)
- int __cdecl _tmain(int argc, TCHAR* argv[]) when called with parameter -r in: Controller_RedirectDevice(argv[2], argv[3]);

Help of **UsbDkController -r ID SN** tells to use "USB\\VID_XXXX&PID_XXXX"


