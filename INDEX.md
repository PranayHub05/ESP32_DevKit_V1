# ESP32 DevKit V1 / ESP32-WROOM-32 — Project Index

This archive contains the projects and experiments documented for the **classic ESP32 DevKit V1 / ESP32-WROOM-32**.

> Scope: classic ESP32 DevKit V1/WROOM-32 only. Arduino UNO R3 and ESP32-S3 projects are intentionally excluded.

## Project status legend

- **TESTED / WORKED** — documented as actually tested or working.
- **DEVELOPED / TESTED** — a working sketch or prototype was developed around the documented hardware.
- **EXPERIMENTAL** — discussed/proposed, but not confirmed as a completed final build.
- **UTILITY** — a hardware/programming utility rather than a standalone end-user project.

## Index

| # | Folder / File | Status | Main purpose |
|---|---|---|---|
| 01 | `01_ESP32_WiFi_Car/ESP32_WiFi_Car.ino` | TESTED | Wi-Fi AP + browser controls for the mini car |
| 02 | `02_ESP32_Single_Motor_WebServer/ESP32_Single_Motor_WebServer.ino` | TESTED | Single-motor PWM/WebServer experiment |
| 03 | `03_ESP32_Sensor_WebSocket_JSON/ESP32_Sensor_WebSocket_JSON.ino` | DEVELOPED / TESTED | DHT11 + MQ135 data as JSON over WebSocket port 81 |
| 04 | `04_ESP32_OpenWeather_Sensor_WebSocket/ESP32_OpenWeather_Sensor_WebSocket.ino` | DEVELOPED / TESTED | Weather API + local sensor data + WebSocket JSON |
| 05 | `05_ESP32_A2DP_Bluetooth_Test/ESP32_A2DP_Bluetooth_Test.ino` | TESTED | Classic Bluetooth A2DP audio output to a speaker |
| 06 | `06_ESP32_CAM_Programming_Utility/README.md` | UTILITY | Using DevKit V1 as USB-UART programmer for ESP32-CAM |
| 07 | `07_ESP32_I2S_A2DP_Bridge_EXPERIMENTAL/ESP32_I2S_A2DP_Bridge_EXPERIMENTAL.ino` | EXPERIMENTAL | Proposed I²S PCM input → Bluetooth A2DP output bridge |

## Important separation

The following were discussed in the wider hardware work but belong to the **ESP32-S3** side and are therefore not included as classic DevKit V1 projects:

- SH1106 OLED + DHT11 + MQ135 + OpenWeather + WebSocket integration when explicitly developed around ESP32-S3.
- ESP32-S3 XiaoZhi audio firmware.
- S3-specific microphone/ML/FreeRTOS work.

The DevKit V1 was also used as a **USB-UART bridge/programmer** for the ESP32-CAM; that is documented separately because it does not require a DevKit V1 application sketch.

## Pin references used in the documented projects

### Car / HW095
- GPIO25 — PWM / ENA-style motor control
- GPIO26 — motor input
- GPIO27 — motor input
- GPIO14 — additional motor input in the two-motor car configuration

### UART / ESP32-CAM programming
- GPIO1 / TX0
- GPIO3 / RX0
- GND common
- 5V/VIN supply path as documented
- IO0 on the camera pulled to GND during flashing

### Experimental I²S bridge
The previously discussed proposal used:
- ESP32-S3 audio data → DevKit V1 GPIO34
- BCLK → GPIO25
- WS/LRC → GPIO26
- Common GND

That bridge was not treated as a completed project.
