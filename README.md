# usbpower

This is a try to develop a Win lower filter driver to patch USB support adding Port-Power control feature to powered USB HUBs which support it (very few). The target is to allow uhubctl https://github.com/mvp/uhubctl to work on Windows.

Power per port means real switch off +5V in the USB ports.

This is my first driver so it will take me some time.
Here below it is what I learned up to now.


## FACTS
In USB messages are send via MessagePipes to Endpoints

Default Control Pipe to Endpoint Zero is owned and managed by USBD (USB Driver)

We need USB Control Transfer to check and use power per port

User space application calls IOCTLs to comunicate with Device Driver (or Device Driver Filter).

There are no IOCTLs to send USB Control Transfer from user space (application)

  The most similar are IRP_MJ_DEVICE_CONTROL:
  
    IOCTL_WRITE_REGISTERS, IOCTL_READ_REGISTERS, IOCTL_SEND_USB_REQUEST: all of them for imaging devices
    
    IOCTL_USB_USER_REQUEST - USBUSER_PASS_THRU: in disuse and it looks like only allowed to admin users
    
There is a IOCTL to send USB packets but it can be used only from kernel mode (driver) it is IOCTL_INTERNAL_USB_SUBMIT_URB


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

Simple URB (USB Request Block) Monitor:
  https://www.codeproject.com/Articles/335364/Simple-URB-USB-Request-Block-Monitor
