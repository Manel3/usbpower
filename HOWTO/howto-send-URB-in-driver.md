## Howto send USB in driver
### Key API definitions and calls:
- IOCTL number: [IOCTL_INTERNAL_USB_SUBMIT_URB](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/usbioctl/ni-usbioctl-ioctl_internal_usb_submit_urb)
- IOCTL call: [IoBuildDeviceIoControlRequest](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/wdm/nf-wdm-iobuilddeviceiocontrolrequest)

### How it is done on:
In UsbDk-1.00-19/UsbDk/**DeviceAccess.cpp**:
``` c
NTSTATUS UsbDkSendUrbSynchronously(PDEVICE_OBJECT Target, URB &Urb)
{
    CIoControlIrp Irp;
    auto status = Irp.Create(Target, IOCTL_INTERNAL_USB_SUBMIT_URB);
    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVACCESS, "%!FUNC! Error %!STATUS! during IOCTL IRP creation", status);
        return status;
    }
    ...
```

In UsbDk-1.00-19/UsbDk/**Irp.cpp**:
``` c
NTSTATUS CIoControlIrp::Create(PDEVICE_OBJECT TargetDevice,
                               ULONG IoControlCode,
                               bool IsInternal,
                               PVOID InputBuffer,
                               ULONG InputBufferLength,
                               PVOID OutputBuffer,
                               ULONG OutputBufferLength)
{
    m_Irp = IoBuildDeviceIoControlRequest(IoControlCode,
                                          TargetDevice,
                                          InputBuffer,
                                          InputBufferLength,
                                          OutputBuffer,
                                          OutputBufferLength,
                                          IsInternal ? TRUE : FALSE,
                                          m_Event,
                                          &m_IoControlStatus);

    if (m_Irp == nullptr)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    ObReferenceObject(TargetDevice);
    m_TargetDevice = TargetDevice;
    return STATUS_SUCCESS;
}
```
