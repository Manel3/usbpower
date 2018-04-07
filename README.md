# usbpower

This is a try to develop a Win lower filter driver to patch USB support adding Port-Power control feature to powered USB HUBs which support it (very few). The target is to allow uhubctl https://github.com/mvp/uhubctl to work on Windows.

Power per port means real switch off +5V in the USB ports.

This is my first driver so it will take me some time.
Here below it is what I am researching.

## HOWTO
KEYS:
- [ ] = TODO (in progress, feel free to help creating an issue!
- [x] = Done

KNOWLEDGE:
- [ ] [howto-put-control-transfer-in-IOCTL](https://github.com/Manel3/usbpower/blob/master/HOWTO/howto-put-control-transfer-in-IOCTL.md)
- [ ] [howto-find-handle-for-an-IOCTL](https://github.com/Manel3/usbpower/blob/master/HOWTO/howto-find-handle-for-an-IOCTL.md)


## FACTS
- In USB messages are send via MessagePipes to Endpoints
- Default Control Pipe to Endpoint Zero is owned and managed by USBD (USB Driver)
- We need USB Control Transfer to check and use power per port
- User space application calls IOCTLs to comunicate with Device Driver (or Device Driver Filter).
- There are no IOCTLs to send USB Control Transfer from user space (application)
- The most similar are IRP_MJ_DEVICE_CONTROL:
  + IOCTL_WRITE_REGISTERS, IOCTL_READ_REGISTERS, IOCTL_SEND_USB_REQUEST: all of them for imaging devices
  + IOCTL_USB_USER_REQUEST - USBUSER_PASS_THRU: in disuse and it looks like only allowed to admin users
- There is a IOCTL to send USB packets but it can be used only from kernel mode (driver) it is IOCTL_INTERNAL_USB_SUBMIT_URB

## INFO drivers
- Getting Started Writing Windows Drivers: https://www.osr.com/getting-started-writing-windows-drivers/
- Writing WDF Drivers I: Core Concepts Lab Material: https://github.com/OSRDrivers/WDF-I
- Best Practices for Windows Driver Developers, in: http://insider.osr.com/2017/ntinsider_2017_01.pdf

## INFO WDF from OSR
- [Bye-Bye Co-Installers](https://www.osr.com/nt-insider/2016-issue1/bye-bye-co-installers/)
- [KMDF and UMDF Hints](https://www.osr.com/nt-insider/2015-issue1/kmdf-umdf-hints/)
- [WDF File Object Callbacks and Properties Demystified](https://www.osr.com/nt-insider/2014-issue1/wdf-file-object-callbacks-properties-demystified/)
- [The Basics About WDF Queues](https://www.osr.com/nt-insider/2011-issue2/basics-wdf-queues/)
- [Driver Signing and Windows 10](https://www.osr.com/nt-insider/2015-issue2/driver-signing-windows-10/)

## INFO WDM (Windows Driver Model?):

I/O Control Operations:
  https://www-user.tu-chemnitz.de/~heha/oney_wdm/ch09d.htm

Working with the Bus Driver:
 https://www-user.tu-chemnitz.de/~heha/oney_wdm/ch11c.htm
Control Transfers:
 https://www-user.tu-chemnitz.de/~heha/oney_wdm/ch11b.htm


## INFO USB:

What is libusb?:
  https://unboxnbeyond.wordpress.com/2014/03/15/what-is-libusb/

USB made simple:
  http://www.usbmadesimple.co.uk/

USB specifications docs:
http://www.usb.org/developers/docs/


MS WDK FORUM:
https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/home?forum=wdk



## WINDOWS API:
DeviceIoControl function:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363216(v=vs.85).aspx

Calling DeviceIoControl:
  https://msdn.microsoft.com/es-es/library/windows/desktop/aa363147(v=vs.85).aspx

Defining I/O Control Codes:
  https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/defining-i-o-control-codes



USB IOCTLs:
  https://msdn.microsoft.com/en-us/library/windows/hardware/ff537421(v=vs.85).aspx

Windows IOCTL Reference:
  http://www.ioctls.net/

IRP_MJ_DEVICE_CONTROL:
  https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/irp-mj-device-control

IOCTL_INTERNAL_USB_SUBMIT_URB (real USB Request Block (packet sent) but only form kernel mode (driver) no from user space:
  https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbioctl/ni-usbioctl-ioctl_internal_usb_submit_urb

IOCTL_USB_USER_REQUEST control code:
  https://msdn.microsoft.com/en-us/library/windows/hardware/ff537344(v=vs.85).aspx

IOCTL_SEND_USB_REQUEST IOCTL:
  https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbscan/ni-usbscan-ioctl_send_usb_request

IOCTL_WRITE_REGISTERS IOCTL:
  https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbscan/ni-usbscan-ioctl_write_registers

IOCTL_READ_REGISTERS IOCTL:
  https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbscan/ni-usbscan-ioctl_read_registers

usbscan.h header:
  https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbscan/



## EXAMPLES:
**UsbDk** direct and exclusive access to USB devices by detaching those from Windows:
- https://github.com/daynix/UsbDk

**libusbK** is a complete driver/library solution for vendor class usb device interfaces: 
- https://sourceforge.net/projects/libusbk/

Simple URB (USB Request Block) Monitor:
- https://www.codeproject.com/Articles/335364/Simple-URB-USB-Request-Block-Monitor
