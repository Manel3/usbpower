## Howto begin a driver
- To create the .cat file (security catalog) it is needed a project "DriverName Package"
- The .inf file selects to which class is attached the filter driver using:
  + Class
  + ClassGuid

| Command        | Explanation                | Found                                                  |
|----------------|----------------------------|--------------------------------------------------------|
| devcon classes | Show setup classes         | USB : Universal Serial Bus controllers  >  Class = USB |
| [setup classes](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/system-defined-device-setup-classes-available-to-vendors) | Find GUID of a setup class | ClassGuid = {36fc9e60-c465-11cf-8056-444553540000} |

### Which class?
| Class     | Description                                         |
|-----------|-----------------------------------------------------|
| USB       | USB host controllers and USB hubs                   |
| USBDevice | all USB devices that do not belong to another class |
| HIDClass  | USB devices that comply with the USB HID Standard   |

### Links
- [System-Defined Device Setup Classes Reserved for System Use](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/system-defined-device-setup-classes-available-to-vendors)
- [System-Defined Device Setup Classes Reserved for System Use](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/system-defined-device-setup-classes-reserved-for-system-use)
