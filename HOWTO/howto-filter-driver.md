## How to design and use a filter driver

Filter drivers:
- can be **upper** and **lower**
- can be for all devices of a **class** or for one **device**
- can be stacked and the order can be changed

#### Driver stack

| APP  | upper class filters | upper device filters | device driver | lower class filters | lower device filters | hardware |
|------|---------------------|----------------------|---------------|---------------------|----------------------|----------|

### Types of drivers

| Type | Description            | Explanation                                                     |
|------|------------------------|-----------------------------------------------------------------|
| UMDF | WDF User Mode Driver   | there is a framework, limited access to kernel                  |
| KMDF | WDF Kernel Mode Driver | there is a framework, kernel mode with access to INTERNAL IOCTL |
| WDS  |  ? | close to the operating system |

The framework does work for you, but you know how to interface with it

### Example from MS
[kbfiltr](https://github.com/Microsoft/Windows-driver-samples/tree/master/input/kbfiltr)
- upper class filter
- creates a raw PDO to open a communication channel to the filter driver

### Links
About filters:

- [Filter Drivers](https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/filter-drivers)
- [Installing a Filter Driver](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/installing-a-filter-driver)

About drivers:
- [Types of WDM Drivers](https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/types-of-wdm-drivers)
- [Differences Between WDM and WDF](https://docs.microsoft.com/en-us/windows-hardware/drivers/wdf/differences-between-wdm-and-kmdf)
- [Choosing a driver model](https://docs.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/choosing-a-driver-model)
- [Write your first driver](https://docs.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/writing-your-first-driver)
- [Developing, Testing, and Deploying Drivers](https://docs.microsoft.com/en-us/windows-hardware/drivers/develop/)
