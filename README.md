# BleKeyboardMOD

This is a **modified version** of the original [`BleKeyboardC6`](https://github.com/T-vK/ESP32-BLE-Keyboard) library, specifically adapted to be **compatible with the USB HID stack** used on the **ESP32-S3**.

## Why was it modified?

The ESP32-S3 supports both **Bluetooth HID** and **USB HID** simultaneously. However, the original BLE and USB libraries define overlapping structures and symbols such as:

- `KeyReport`
- Functions like `press()`, `release()`, `write()`
- Class names like `BleKeyboard`

This causes **symbol redefinition conflicts** during compilation when both libraries are included in the same project.

## What was changed?

To prevent conflicts, the following changes were made to this version:

| Original Element | Renamed Version           |
|------------------|---------------------------|
| `BleKeyboard`    | `BleKeyboardBLE`          |
| `KeyReport`      | `KeyReportBLE`            |
| `press()`        | `pressBLE()` (if needed)  |
| `release()`      | `releaseBLE()` (if needed)|

> *All renamed elements are isolated to allow coexistence with `USBHIDKeyboard` without symbol collisions.*

## Typical usage

```cpp
#include <USBHIDKeyboard.h>     // For USB HID
#include <BleKeyboardMOD.h>     // This modified BLE HID version

USBHIDKeyboard usbKeyboard;
BleKeyboardBLE bleKeyboard;
```

## Credits
This library is based on the excellent work by [`T-vK`](https://github.com/siroitori0413/), author of the original
[`ESP32-BLE-Keyboard`](https://github.com/siroitori0413/ESP32-BLE-Keyboard) library.