# Application to demonstrate Mbed-OS crash reporting feature

This example demonstrates how Mbed OS crash-reporting works and the new APIs associated with it.

**Table of contents:**

1. [Requirements](#requirements)
1. [Setup](#setup)
   - [Clone the example](#import-the-example)
   - [Compile the example](#compile-the-example)
   - [Run the example](#run-the-example)

## Requirements

This example program uses K64F as the test device.


## Setup

#### Clone the example

Setup your Mbed development environment as described in - [Get started with Mbed OS](https://os.mbed.com/docs/latest/tutorials/your-first-program.html).

From the command-line, clone the example:

```
git clone https://github.com/ARMmbed/mbed-os-example-crash-reporting
cd mbed-os-example-crash-reporting
mbed deploy
```

#### Description

This example demonstrates Mbed OS crash-reporting feature and the new APIs associated with it. If you look at the main.cpp, you will see a
function with signature `void mbed_error_reboot_callback(mbed_error_ctx *error_context)`. This function is the callback called during Mbed-OS 
initialization if the system rebooted due to a fatal error. This function is called with a pointer to `mbed_error_ctx` structure which captures the
error-context which triggered the reboot. When you run this program(see `main.cpp`), it will force an exception,
which causes an auto-reboot(warm-reset) of the system. Note that the system captures the error-context in a special location in RAM before 
triggering the auto-reboot. During the reboot, the system will detect that the reboot was caused by a fatal error and it calls the 
`mbed_error_reboot_callback()` with a pointer to the error context captured in RAM. The callback function records the fact that a reboot
occurred using a local variable named `reboot_error_happened` and it captures the `mbed_error_ctx` using `mbed_get_reboot_error_info()`
and resets the saved context captured by the system using `mbed_reset_reboot_error_info()`. The system continues to boot and it eventually enters
application `main()`. Note that this is the second time the `main()` is getting called after reboot. This time, `main()` detects that a reboot
using `reboot_error_happened` and it prints out the error context information to the terminal.

#### Compile the example

Compile the application as below using ARM compiler. If you want to use GCC or IAR toolchains, use GCC_ARM or IAR for -t option.

```
mbed compile -m K64F -t ARM
```

This may take few minutes and once its successfully completed it would generate the output as below.

```
Link: mbed-os-example-crash-reporting
Elf2Bin: mbed-os-example-crash-reporting
+------------------+-------+-------+------+
| Module           | .text | .data | .bss |
+------------------+-------+-------+------+
| [lib]\c_w.l      | 12963 |    16 |  348 |
| [lib]\cpprt_w.l  |    42 |     0 |    0 |
| [lib]\fz_wm.l    |    18 |     0 |    0 |
| [lib]\m_wm.l     |    48 |     0 |    0 |
| anon$$obj.o      |    32 |     0 | 1024 |
| main.o           |  2057 |     0 |  268 |
| mbed-os\drivers  |   130 |     0 |    0 |
| mbed-os\features | 16840 |     0 |  304 |
| mbed-os\hal      |  1660 |    30 |   64 |
| mbed-os\platform |  4565 |   104 |  604 |
| mbed-os\rtos     | 14646 |  2310 | 4592 |
| mbed-os\targets  |  9249 |   104 |  324 |
| Subtotals        | 62250 |  2564 | 7528 |
+------------------+-------+-------+------+
Total Static RAM memory (data + bss): 10092 bytes
Total Flash memory (text + data): 64814 bytes

Image: .\BUILD\k64f\arm\mbed-os-example-crash-reporting.bin
```

#### Run the example

1. Connect a K64F device to your pc
1. Open a serial terminal connected to the device
1. Copy the binary file to the device.
1. Reset the device by pressing the reset button to start the program.

The output should look similar to below:

```

Mbed-OS crash reporting test

Forcing exception

++ MbedOS Fault Handler ++

FaultType: HardFault

Context:
R0   : 0000AAA3
R1   : 00000208
R2   : 00005D70
R3   : 00004B95
R4   : 00000000
R5   : 00000000
R6   : 00000000
R7   : 00000000
R8   : 00000000
R9   : 00000000
R10  : 00000000
R11  : 00000000
R12  : 00008759
SP   : 20001FB0
LR   : 00002C59
PC   : 00005CB8
xPSR : 01000000
PSP  : 20001F48
MSP  : 2002FFD8
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
Location: 0x5F3F
Error Value: 0x5CB8
Current Thread: main  Id: 0x20001FC0 Entry: 0x64F9 StackSize: 0x1000 StackMem: 0x20000FC0 SP: 0x2002FF70
For more info, visit: https://armmbed.github.io/mbedos-error/?error=0x80FF013D
-- MbedOS Error Info -
== Your last reboot was triggered by an error, below is the error information ==

++ MbedOS Error Info ++
Error Status: 0x80FF013D Code: 317 Module: 255
Error Message: System rebooted due to fatal error
Location: 0x5F3F
File: mbed_error.c+211
Error Value: 0x5CB8
Current Thread: main  Id: 0x20001FC0 Entry: 0x64F9 StackSize: 0x1000 StackMem: 0x20000FC0 SP: 0x2002FF70
For more info, visit: https://armmbed.github.io/mbedos-error/?error=0x80FF013D
-- MbedOS Error Info --

== Reboot count(=1) exceeded maximum, system halting ==
Reboor error callback received

```

#### Referenes
https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/platform/crash-reporting/crash_reporting.md

## License and contributions

The software is provided under Apache-2.0 license. Contributions to this project are accepted under the same license. Please see [contributing.md](CONTRIBUTING.md) for more info.

This project contains code from other projects. The original license text is included in those source files. They must comply with our license guide.

