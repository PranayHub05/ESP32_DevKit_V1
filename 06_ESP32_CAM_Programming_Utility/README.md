# ESP32 DevKit V1 as ESP32-CAM USB-UART Programmer

Status: UTILITY / TESTED

The classic ESP32 DevKit V1 was used as a USB-UART bridge to program an ESP32-CAM.

## DevKit UART0

- GPIO1 = TX0
- GPIO3 = RX0

## Cross connection

```text
ESP32 DevKit V1 TX0 / GPIO1  -> ESP32-CAM U0R
ESP32 DevKit V1 RX0 / GPIO3  <- ESP32-CAM U0T
GND                         -> GND
```

The camera board also needs appropriate power.

## Flash mode

Connect:

```text
ESP32-CAM IO0 -> GND
```

before resetting/starting the upload.

After flashing:

1. Stop the upload.
2. Remove IO0 from GND.
3. Reset the camera.
4. The normal application should boot.

## Why this is not an .ino project

The DevKit is being used as the serial interface. Its own firmware is not the main application.

Therefore this folder intentionally contains documentation rather than a standalone DevKit sketch.

## Camera note

The camera board used in the work was identified as a RHYX M21-45. Standard camera examples did not immediately match its sensor configuration, so camera-specific firmware required separate handling.
