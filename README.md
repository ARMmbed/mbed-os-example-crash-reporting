![](./resources/official_armmbed_example_badge.png)
# Crash reporting Mbed OS example

This example demonstrates how Mbed OS crash reporting works on an Mbed OS Enabled platform.

You can find more information about the crash reporting APIs in the [documentation](https://os.mbed.com/docs/mbed-os/latest/apis/error-handling.html#crash-reporting-and-auto-reboot).

You can build the project with all supported [Mbed OS build tools](https://os.mbed.com/docs/mbed-os/latest/tools/index.html). However, this example project specifically refers to the command-line interface tools, [Arm Mbed CLI 1](https://github.com/ARMmbed/mbed-cli#installing-mbed-cli) and [Mbed CLI 2](https://github.com/ARMmbed/mbed-tools#installation).

## Mbed OS build tools

### Mbed CLI 2
Starting with version 6.5, Mbed OS uses Mbed CLI 2. It uses Ninja as a build system, and CMake to generate the build environment and manage the build process in a compiler-independent manner. If you are working with Mbed OS version prior to 6.5 then check the section [Mbed CLI 1](#mbed-cli-1).
1. [Install Mbed CLI 2](https://os.mbed.com/docs/mbed-os/latest/build-tools/install-or-upgrade.html).
1. From the command-line, import the example: `mbed-tools import mbed-os-example-crash-reporting`
1. Change the current directory to where the project was imported.

### Mbed CLI 1
1. [Install Mbed CLI 1](https://os.mbed.com/docs/mbed-os/latest/quick-start/offline-with-mbed-cli.html).
1. From the command-line, import the example: `mbed import mbed-os-example-crash-reporting`
1. Change the current directory to where the project was imported.


## Application functionality

This example demonstrates the Mbed OS crash reporting feature and the APIs associated with it. In the first run, the application generates a fault that causes the system to reboot. In the second run, the application detects that the reboot was caused by an error and prints the fault context information in the console.

First run:

1. During Mbed OS initialization, the system checks the Crash-data-RAM region and detects there was no fault.
1. The startup code calls `main()`.
   1. The application generates an exception.
   1. The system captures the error-context in a special location in RAM.
1. The system auto-reboots (warm-resets).

Second run:

1. During Mbed OS initialization, the system:
   1. Checks the Crash-data-RAM region.
   1. Detects that the reboot was due to a fatal error.
   1. Calls `mbed_error_reboot_callback()` with a pointer to the error context stored in RAM.
   
   In this example, the application has overridden the callback function (WEAK function). The function:
      1. Sets a global variable `reboot_error_happened` to record that the reboot was due to an error.
      1. Prints information about the error context in the console. 
      1. Resets the saved context captured by the system in RAM using `mbed_reset_reboot_error_info()`.
      
1. The startup code calls `main()`. The application:
   1. Checks `reboot_error_happened`.
   1. Retrieves the fault context using `mbed_get_reboot_fault_context()`.
   1. Prints the fault context.

## Building and running

1. Connect a USB cable between the USB port on the target and the host computer.
1. Run this command to build the example project and program the microcontroller flash memory:

    * Mbed CLI 2

    ```bash
    $ mbed-tools compile -m <TARGET> -t <TOOLCHAIN> --flash
    ```

    * Mbed CLI 1

    ```bash
    $ mbed compile -m <TARGET> -t <TOOLCHAIN> --flash
    ```

(Note: You can use the Mbed CLI command-line option `--sterm` to open a serial terminal after flashing.)

Your PC may take a few minutes to compile your code.

The binary is located at:

* **Mbed CLI 2** -
  `./cmake_build/<TARGET>/develop/<TOOLCHAIN>/mbed-os-example-crash-reporting.bin`

* **Mbed CLI 1** - `./BUILD/<TARGET>/<TOOLCHAIN>/mbed-os-example-crash-reporting.bin`.

You can manually copy the binary to the target, which gets mounted on the host
computer through USB, rather than using the `--flash` option.

You can also open a serial terminal separately, rather than using the `--sterm`
option, with the following command:

* Mbed CLI 2
    ```bash
    $ mbed-tools sterm
    ```

* Mbed CLI 1
    ```bash
    $ mbed sterm
    ```

Depending on the target, you can build the example project with the `GCC_ARM` or `ARM` toolchain.

## Expected output

The serial terminal shows an output similar to:

```
--- Terminal on /dev/tty.usbmodem11102 - 9600,8,N,1 ---

This is the crash reporting Mbed OS example
1st run: Inject the fault exception

++ MbedOS Fault Handler ++

FaultType: HardFault

Context:
R0   : 0000DE54
R1   : 00000000
R2   : E000ED00
R3   : 0000AAA3
R4   : 00000000
R5   : 00000000
R6   : 00000000
R7   : 00000000
R8   : 00000000
R9   : 00000000
R10  : 00000000
R11  : 00000000
R12  : FFFFFFFF
SP   : 20002E38
LR   : 0000181F
PC   : 0000178C
xPSR : 010F0000
PSP  : 20002E18
MSP  : 2002FFC0
CPUID: 410FC241
HFSR : 40000000
MMFSR: 00000000
BFSR : 00000000
UFSR : 00000100
DFSR : 00000008
AFSR : 00000000
Mode : Thread
Priv : Privileged
Stack: PSP

-- MbedOS Fault Handler --



++ MbedOS Error Info ++
Error Status: 0x80FF013D Code: 317 Module: 255
Error Message: Fault exception
Location: 0x178C
Error Value: 0x1FFF0400
Current Thread: main Id: 0x20000E68 Entry: 0x31E7 StackSize: 0x1000 StackMem: 0x20001E60 SP: 0x20002E38
For more info, visit: https://mbed.com/s/error?error=0x80FF013D&tgt=K64F
-- MbedOS Error Info --

= System will be rebooted due to a fatal error =

(before main) mbed_error_reboot_callback invoked with the following error context:
    Status      : 0x80FF013D
    Value       : 0x1FFF0400
    Address     : 0x178C
    Reboot count: 0x1
    CRC         : 0x60F72ECE

This is the crash reporting Mbed OS example.
2nd run: Retrieve the fault context using mbed_get_reboot_fault_context
    R0   : 0xDE54
    R1   : 0x0
    R2   : 0xE000ED00
    R3   : 0xAAA3
    R4   : 0x0
    R5   : 0x0
    R6   : 0x0
    R7   : 0x0
    R8   : 0x0
    R9   : 0x0
    R10   : 0x0
    R11   : 0x0
    R12   : 0xFFFFFFFF
    SP   : 0x20002E38
    LR   : 0x181F
    PC   : 0x178C
    xPSR : 0x10F0000
    PSP  : 0x20002E18
    MSP  : 0x2002FFC0

Mbed OS crash reporting example completed
```

## Configuring the application

You can enable the crash reporting feature by setting `platform.crash-capture-enabled` to true in the application configuration file:

```
{
    "target_overrides": {
        "*": {            
            "platform.crash-capture-enabled": true            
        }
    }
}
```

This feature is enabled on some targets by default. To enable it on your target, modify the scatter file to reserve a dedicated region in RAM. You can find more details in the [documentation](https://os.mbed.com/docs/mbed-os/latest/apis/error-handling.html#crash-reporting-and-auto-reboot).

## Using mbed_get_reboot_error_info() to retrieve the reboot error context

You can retrieve the reboot error context in two ways:

- Using `mbed_error_reboot_callback()` as shown in this example.
- Using `mbed_get_reboot_error_info()` as shown below:

   ```
   int main()
   {
       mbed_error_ctx error_context;
       if (mbed_get_reboot_error_info(&error_context) != MBED_ERROR_ITEM_NOT_FOUND) {
           printf("Retrieved reboot error context\n");
               
           mbed_fault_context_t fault_ctx;
           mbed_get_reboot_fault_context(&fault_ctx);
           printf("Retrieved fault context\n");
       }
   }
   ```

## Troubleshooting 

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it. 

## Related links

- [Mbed OS configuration](https://os.mbed.com/docs/latest/reference/configuration.html).
- [Mbed OS serial communication](https://os.mbed.com/docs/latest/tutorials/serial-communication.html).
- [Mbed boards](https://os.mbed.com/platforms/).

### License and contributions

The software is provided under the Apache-2.0 license. Contributions to this project are accepted under the same license. Please see contributing.md for more information.

This project contains code from other projects. The original license text is included in those source files. They must comply with our license guide.

