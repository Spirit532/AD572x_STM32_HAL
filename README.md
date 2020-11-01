# AD572x.h
A driver stub for AD572x digital potentiometers using STM32's HAL

Usage:
```C++
AD527X_enable_wiper(&hi2c1, AD527X_ADDRESS_GND);
AD527X_write_rdac(&hi2c1, AD527X_ADDRESS_GND, 111);
bool ok = (AD527X_read_rdac(&hi2c1, AD527X_ADDRESS_GND) == 111);
```
