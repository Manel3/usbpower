## Howto put control transfer in IOCTL

We could create specific IOCTL for the commands (bRequest control transfers) we need (SET_FEATURE and CLEAR_FEATURE) or use a generic way.

### How it is done on:
I think libusb in Linux does it in **submit_control_transfer()** in [/libusb/os/linux_usbfs.c#L2184](https://github.com/libusb/libusb/blob/master/libusb/os/linux_usbfs.c#L2184)

``` c
ioctl(dpriv->fd, IOCTL_USBFS_SUBMITURB, urb);
```

**IOCTL_USBFS_SUBMITURB** is defined in [/libusb/os/linux_usbfs.h#L152](https://github.com/libusb/libusb/blob/master/libusb/os/linux_usbfs.h#L152)

``` c
#define IOCTL_USBFS_SUBMITURB	_IOR('U', 10, struct usbfs_urb)
```

``` c
struct usbfs_urb {
	unsigned char type;
	unsigned char endpoint;
	int status;
	unsigned int flags;
	void *buffer;
	int buffer_length;
	int actual_length;
	int start_frame;
	union {
		int number_of_packets;	/* Only used for isoc urbs */
		unsigned int stream_id;	/* Only used with bulk streams */
	};
	int error_count;
	unsigned int signr;
	void *usercontext;
	struct usbfs_iso_packet_desc iso_frame_desc[0];
};
```
