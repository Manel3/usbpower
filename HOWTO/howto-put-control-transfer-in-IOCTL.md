## Howto put control transfer in IOCTL

We could create specific IOCTL for the commands (bRequest control transfers) we need (SET_FEATURE and CLEAR_FEATURE) or use a generic way.

### How it is done on:
I think libusb in Linux does it in **submit_control_transfer()** in [/libusb/os/linux_usbfs.c#L2184](https://github.com/libusb/libusb/blob/master/libusb/os/linux_usbfs.c#L2184)


    ioctl(dpriv->fd, IOCTL_USBFS_SUBMITURB, urb);

**IOCTL_USBFS_SUBMITURB** is defined in [/libusb/os/linux_usbfs.h#L152](https://github.com/libusb/libusb/blob/master/libusb/os/linux_usbfs.h#L152)

    #define IOCTL_USBFS_SUBMITURB	_IOR('U', 10, struct usbfs_urb)
    
