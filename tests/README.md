# Testing examples

Examples are tested using tool [htrun](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-host-tests) and templated print log. 

To run the test, use following command after you build the example:
This assumes that target K64F(DUT) with a mount point of D: and a serial port on COM4

```
mbedhtrun -d D: -p COM4 -m K64F -f .\BUILD\K64F\GCC_ARM\crash-reporting.bin --compare-log tests\crash-reporting.log
```


More details about `htrun` are [here](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-host-tests#testing-mbed-os-examples).
