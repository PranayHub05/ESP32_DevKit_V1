# ESP32 DevKit V1 Bluetooth Classic A2DP Test

Status: TESTED / WORKING

## Purpose

Test the classic ESP32's Bluetooth Classic A2DP audio capability.

The documented result was successful playback through a Bluetooth speaker.

## Platform

This project is specifically for the classic ESP32/WROOM-32 family.

## Library

One commonly used Arduino implementation is:

`ESP32-A2DP`

The exact library version can affect the API. If the installed library exposes a different API, use its examples as the reference.

## Important

A2DP is Bluetooth Classic functionality. Do not assume that code written for classic ESP32 Bluetooth will work unchanged on ESP32-S3.

## Test

1. Install a compatible ESP32 A2DP library.
2. Replace the speaker name in the sketch.
3. Upload to the classic ESP32.
4. Open Serial Monitor at 115200.
5. Put the Bluetooth speaker in pairing mode.
6. Observe the connection and audio output.

The included sketch is a minimal test/starting point; the exact audio source can be replaced by microphone/I²S/PCM data later.
