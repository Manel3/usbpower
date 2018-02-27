## Howto find handle for an IOCTL

DeviceIoControl need a handle (named hDevice) to determine the device.

Doc below tells to use the CreateFile function to retrieve it. I am not sure that is the way for a filter driver.

TODO


## WINDOWS API:
DeviceIoControl function:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363216(v=vs.85).aspx

Calling DeviceIoControl:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363147(v=vs.85).aspx
