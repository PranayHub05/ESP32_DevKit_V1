# Experimental I²S → A2DP Bridge

Status: EXPERIMENTAL / NOT CONFIRMED COMPLETE

## Goal

Receive PCM audio from an ESP32-S3/XiaoZhi source through I²S and transmit it from the classic ESP32 DevKit V1 over Bluetooth Classic A2DP to a Bluetooth speaker.

## Proposed architecture

```text
ESP32-S3 / XiaoZhi
      │
      │ I²S PCM
      ▼
ESP32 DevKit V1
      │
      │ Bluetooth Classic A2DP
      ▼
Bluetooth Speaker
```

## Proposed wiring

| Signal | Source | DevKit V1 |
|---|---:|---:|
| DATA | GPIO7 | GPIO34 |
| BCLK | GPIO15 | GPIO25 |
| WS/LRC | GPIO16 | GPIO26 |
| GND | GND | GND |

These were proposed test pins, not universal requirements.

## Main engineering problem

The source and A2DP output need compatible:

- sample rate;
- bit depth;
- channel count;
- I²S format;
- buffering;
- timing.

A source configuration such as 24 kHz must be handled correctly before feeding an A2DP implementation that expects another PCM format.

This project remains clearly marked experimental because the complete end-to-end bridge was not confirmed as finished.
