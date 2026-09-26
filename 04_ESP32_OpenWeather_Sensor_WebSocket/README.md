# ESP32 OpenWeather + DHT11 + MQ135 + WebSocket

Status: DEVELOPED / TESTED ARCHITECTURE

## Purpose

Combine:

1. local DHT11 readings;
2. local MQ135 analog data;
3. OpenWeather API data;

and expose the combined information as JSON through WebSocket port 81.

## Credentials

Set:

- Wi-Fi SSID
- Wi-Fi password
- OpenWeather API key
- city/location query

Do not publish your API key to GitHub.

## Architecture

```text
DHT11 ──────┐
MQ135 ──────┤
            ├── ESP32 ── Wi-Fi ── OpenWeather
            │
            └── JSON ── WebSocket :81
```

## Libraries

- WiFi.h
- HTTPClient.h
- WebSocketsServer.h
- ArduinoJson.h
- DHT.h

## Important note

This file is a documented/reconstructed implementation of the architecture discussed for the classic ESP32. The exact historical GPIO assignment and exact API JSON field set were not preserved, so configurable constants are provided.
