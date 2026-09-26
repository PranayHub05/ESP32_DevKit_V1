# ESP32 DHT11 + MQ135 WebSocket JSON

Status: DEVELOPED / TESTED ARCHITECTURE

## Purpose

Read DHT11 and MQ135 data on the classic ESP32 and publish the readings over a WebSocket server on port 81.

## Data flow

DHT11 + MQ135 -> ESP32 -> WebSocket :81 -> phone/tablet/client

## Libraries

Typical Arduino libraries:

- WiFi.h (ESP32 core)
- WebSocketsServer.h
- DHT.h
- ArduinoJson.h

Install compatible versions through Arduino Library Manager if required.

## Pin configuration

The exact historical GPIO numbers for the sensor wiring were not preserved in the available project record, so this sketch uses configurable constants at the top. Set them to the pins used on your physical build before uploading.

## Example JSON

```json
{
  "device": "ESP32",
  "id": "ESP32-WROOM-01",
  "temperature": 25.4,
  "humidity": 61.0,
  "mq135": 1234
}
```
