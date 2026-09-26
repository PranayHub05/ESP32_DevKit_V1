# ESP32 DevKit V1 / ESP32-WROOM-32 - Complete Project README

## 1. Overview

This repository is a consolidated record of the work done with the **classic ESP32 DevKit V1 / ESP32-WROOM-32**.

The purpose of this archive is to preserve the projects as individual Arduino sketches while also documenting:

- what each project does
- the hardware used
- wiring and pin assignments
- software/libraries involved
- how the program works
- how the projects evolved
- what was actually tested
- what was experimental
- and which work belongs to the ESP32-S3 rather than the classic ESP32.

This README is deliberately detailed so that the repository can be used as a learning log as well as a future reference.

---

# 2. Hardware Platform

## Main board

**ESP32 DevKit V1 / ESP32-WROOM-32**

This is the original/classic ESP32 board you used for the relevant projects. It is different from the ESP32-S3 boards used in some of the later sensor and TinyML work.

The classic ESP32 was particularly useful here because it provides both:

- Wi-Fi
- Bluetooth Classic

That combination enabled the car, WebServer/WebSocket experiments, Internet API work, and Bluetooth A2DP audio work.

---

# 3. Project Map

## 01 Wi-Fi Controlled Mini Car

### Status
**TESTED / DEVELOPED**

### Goal

Build a small controllable car using:

- ESP32 DevKit V1
- HW095 motor driver
- two geared DC/TT motors
- battery pack
- a browser-based control interface

The ESP32 creates its own Wi-Fi access point. A phone connects directly to the ESP32 and opens its local control page.

### Network

The documented configuration used:

- SSID: `ESP32-CAR`
- Password: `12345678`
- Typical ESP32 AP address: `192.168.4.1`

### Control pins

The documented two-motor configuration used:

| ESP32 GPIO | Function |
|---|---|
| GPIO25 | Motor control / PWM-related output |
| GPIO26 | HW095 motor input |
| GPIO27 | HW095 motor input |
| GPIO14 | HW095 motor input |

The exact motor-driver board labeling can vary between HW095 variants, so the physical motor-driver labels should always be checked before powering the motors.

### UI behavior

The browser interface was designed around momentary controls:

- forward
- backward
- left
- right
- stop

The control system used a short command heartbeat. Releasing a control caused the system to issue a STOP command.

This is an important safety feature for a remotely controlled car: if the phone stops sending commands, the car should not continue indefinitely.

### Lessons from this project

This project introduced several important ESP32 concepts:

1. Creating a Wi-Fi Access Point.
2. Running an HTTP server.
3. Serving HTML directly from the ESP32.
4. Receiving browser requests.
5. Controlling GPIO outputs.
6. PWM-based motor control.
7. Implementing a browser control interface.
8. Using a heartbeat/watchdog-like command timeout.
9. Separating low-voltage logic from motor power.

---

# 4. Project 02 Single Motor WebServer / PWM Experiment

### Status
**TESTED**

This was a smaller motor-control experiment used to isolate the motor-control side before operating the complete car.

### Documented pin configuration

| ESP32 GPIO | Function |
|---|---|
| GPIO25 | ENA / PWM |
| GPIO26 | IN1 |
| GPIO27 | IN2 |

The right motor was connected to the corresponding motor-driver output pair.

### Wi-Fi configuration

- SSID: `ESP32-MOTOR`
- AP address: `192.168.4.1`

### Interface

The browser page included:

- a speed slider;
- up/forward control;
- down/reverse control;
- STOP.

The PWM output was deliberately capped at approximately **106/255** in the documented experiment.

### Why this project matters

Testing one motor independently is useful because it separates:

- ESP32 software problems;
- motor-driver wiring problems;
- power-supply problems;
- and mechanical problems.

This became especially useful during the later two-motor car work where motor direction and driver outputs had to be debugged separately.

---

# 5. Project 03 DHT11 + MQ135 Sensor WebSocket JSON

### Status
**DEVELOPED / TESTED**

This project was part of the ESP32 sensor-data architecture.

### Goal

Read local sensor values and make them available to another device/application over Wi-Fi.

The intended data path was:

```text
DHT11 ─────┐
           ├──> ESP32 ───> WebSocket server :81 ───> Client
MQ135 ─────┘
```

### Sensors

#### DHT11

Used for environmental readings such as:

- temperature;
- humidity.

#### MQ135

Used as an air-quality/gas sensor.

Important: MQ135 is not a laboratory-grade pollutant analyzer. Its analog value is affected by sensor calibration, warm-up time, supply conditions, environment, and the exact module/circuit.

Therefore the project should treat MQ135 values as a prototype air-quality indicator rather than an exact ppm measurement unless a proper calibration model has been established.

### Communication

The ESP32 acts as a WebSocket server using:

```text
Port: 81
```

The sensor data is packaged into JSON.

A representative structure is:

```json
{
  "device": "ESP32",
  "id": "ESP32-WROOM-01",
  "temperature": 25.4,
  "humidity": 61.0,
  "mq135": 1234
}
```

The exact field names can be modified to match the receiving application.

### Why WebSocket?

Normal HTTP is request/response oriented.

WebSocket provides a persistent connection:

```text
ESP32 <====================> App
       continuous connection
```

This makes it convenient for dashboards and live sensor displays.

### Learning outcomes

This project combined:

- GPIO/ADC;
- sensor libraries;
- Wi-Fi;
- WebSockets;
- JSON;
- real-time data transfer.

That is a significant step up from a basic sensor sketch.

---

# 6. Project 04 OpenWeather + Local Sensors + WebSocket

### Status
**DEVELOPED / TESTED ARCHITECTURE**

This project extended the sensor WebSocket system by combining locally measured data with Internet weather information.

### Data sources

Local:

- DHT11
- MQ135

Internet:

- OpenWeather API

### Conceptual architecture

```text
                ┌──────── DHT11
                │
ESP32 ──────────┼──────── MQ135
                │
                └──────── Wi-Fi ───── OpenWeather API

                     │
                     ▼
              Combined JSON
                     │
                     ▼
             WebSocket :81
                     │
                     ▼
                Phone / App
```

### Why combine them?

The idea was to give the application both:

**Local conditions**
- temperature
- humidity
- air-quality sensor value

and

**External weather information**
- current weather
- weather description
- additional weather parameters returned by the API.

This creates a more useful environmental dashboard than either source alone.

### API security

The API key should not be permanently exposed in a public Git repository.

For a private prototype, a key can be placed in a configuration section, but for a production application the API call should ideally be handled by a backend or protected service.

### JSON design

A useful combined structure is:

```json
{
  "device": {
    "name": "ESP32",
    "id": "ESP32-WROOM-01"
  },
  "sensor": {
    "temperature": 25.4,
    "humidity": 61.0,
    "mq135": 1234
  },
  "weather": {
    "temperature": 26.1,
    "humidity": 58,
    "description": "clear sky"
  }
}
```

This makes the data easier for a phone/tablet application to consume.

---

# 7. Project 05 Bluetooth Classic A2DP Audio

### Status
**TESTED / WORKING**

This was one of the important reasons to use the classic ESP32 rather than an ESP32-S3.

The original ESP32 DevKit V1 was confirmed to work with **Bluetooth Classic A2DP**, and the audio output was successfully tested with a Bluetooth speaker.

### What A2DP means

A2DP stands for:

**Advanced Audio Distribution Profile**

It is a Bluetooth Classic profile designed for streaming audio.

The basic path is:

```text
Audio PCM
   │
   ▼
ESP32
   │
   │ Bluetooth Classic A2DP
   ▼
Bluetooth Speaker
```

### Why this matters

Many newer ESP32-family chips do not have the same Bluetooth Classic support as the original ESP32.

The classic ESP32/WROOM platform was therefore useful for this particular experiment.

### Typical audio parameters

The previously discussed A2DP setup used conventional PCM audio concepts such as:

- stereo;
- 16-bit samples;
- commonly 44.1 kHz for A2DP source applications.

The exact source format must match the A2DP implementation.

### Result

The Bluetooth speaker playback was confirmed to work well with the classic ESP32.

---

# 8. Project 06 ESP32 DevKit V1 as USB-UART Programmer for ESP32-CAM

### Status
**UTILITY / TESTED**

The DevKit V1 was also used as a programming interface for the ESP32-CAM.

This is not a normal application running on the DevKit. Instead, the DevKit is being used as a USB-to-serial bridge.

### Serial pins

Classic ESP32 UART0:

| DevKit V1 | Function |
|---|---|
| GPIO1 | TX0 |
| GPIO3 | RX0 |

The serial connection is crossed:

```text
DevKit TX0 (GPIO1) → ESP32-CAM U0R
DevKit RX0 (GPIO3) ← ESP32-CAM U0T
GND                 → GND
```

### Flashing

For the camera board:

```text
IO0 → GND
```

is used during the flashing process so that the ESP32-CAM enters bootloader/download mode.

After uploading, IO0 should be released from GND and the camera reset.

### Why use the DevKit?

The DevKit already provides a USB serial interface.

Therefore it can be used as a convenient bridge when a camera board does not have its own USB connector.

---

# 9. Project 07 I²S Audio → Bluetooth A2DP Bridge

### Status
**EXPERIMENTAL / NOT CONFIRMED AS A FINISHED PROJECT**

This project should not be represented as completed.

The idea was to connect an ESP32-S3/XiaoZhi audio source to the classic ESP32 DevKit V1.

The proposed architecture was:

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

### Previously discussed proposed wiring

| Signal | S3 side | DevKit V1 |
|---|---:|---:|
| I²S data | GPIO7 | GPIO34 |
| BCLK | GPIO15 | GPIO25 |
| WS/LRC | GPIO16 | GPIO26 |
| GND | GND | GND |

These pins were part of a proposed design and should not be treated as universally correct for every XiaoZhi firmware configuration.

### Main technical challenge

The audio source and receiver must agree on:

- sample rate;
- sample width;
- number of channels;
- I²S timing;
- data alignment;
- buffering.

For example, a source configured for 24 kHz cannot simply be assumed to match an A2DP pipeline expecting another format.

### Status

The architecture was discussed, but the exact end-to-end S3 → I²S → DevKit → A2DP implementation was not established as a finished tested project.

---

# 11. Important Hardware Lessons From the Car Project

The motor experiments produced several practical lessons.

## Motor voltage

The geared TT motors were intended for low voltage operation.

Applying a much higher voltage can cause:

- excessive current;
- motor heating;
- driver heating;
- battery stress;
- mechanical damage.

The ESP32 GPIO pins must never directly drive the motors.

Correct architecture:

```text
ESP32 GPIO
   │
   ▼
Motor Driver
   │
   ▼
Motor
```

not:

```text
ESP32 GPIO ─── Motor
```

## Separate logic and motor power

The ESP32 should receive a clean suitable supply.

The motors can create electrical noise and large current changes, so the motor supply should be designed separately from the ESP32 logic supply where practical, while maintaining a common signal ground.

---

# 12. WebServer vs WebSocket

Two communication approaches appeared in the projects.

## HTTP WebServer

Good for:

- buttons;
- sliders;
- control pages;
- configuration.

Example:

```text
Phone
  │
  │ HTTP
  ▼
ESP32 WebServer
```

## WebSocket

Good for:

- continuous sensor updates;
- live dashboards;
- low-latency two-way communication.

Example:

```text
Phone/App
   ⇅
WebSocket
   ⇅
ESP32
```

The car project mainly used HTTP/WebServer control, while the sensor architecture used WebSocket on port 81.

---

# 13. Suggested Repository Structure

```text
ESP32_DevKit_V1_Projects/
│
├── README.md
├── INDEX.md
│
├── 01_ESP32_WiFi_Car/
│   ├── ESP32_WiFi_Car.ino
│   └── README.md
│
├── 02_ESP32_Single_Motor_WebServer/
│   ├── ESP32_Single_Motor_WebServer.ino
│   └── README.md
│
├── 03_ESP32_Sensor_WebSocket_JSON/
│   ├── ESP32_Sensor_WebSocket_JSON.ino
│   └── README.md
│
├── 04_ESP32_OpenWeather_Sensor_WebSocket/
│   ├── ESP32_OpenWeather_Sensor_WebSocket.ino
│   └── README.md
│
├── 05_ESP32_A2DP_Bluetooth_Test/
│   ├── ESP32_A2DP_Bluetooth_Test.ino
│   └── README.md
│
├── 06_ESP32_CAM_Programming_Utility/
│   └── README.md
│
└── 07_ESP32_I2S_A2DP_Bridge_EXPERIMENTAL/
    ├── ESP32_I2S_A2DP_Bridge_EXPERIMENTAL.ino
    └── README.md
```

---

# 14. Recommended Learning Progression

The projects form a natural progression:

```text
GPIO
 │
 ├── Motor control
 │
 ▼
Wi-Fi
 │
 ├── WebServer
 │
 ▼
Real-time networking
 │
 ├── WebSocket
 │
 ▼
Sensor integration
 │
 ├── DHT11
 ├── MQ135
 │
 ▼
Internet APIs
 │
 └── OpenWeather
 │
 ▼
Structured data
 │
 └── JSON
 │
 ▼
Bluetooth
 │
 └── A2DP
 │
 ▼
Digital audio
 │
 └── I²S
```

This is a useful progression from embedded basics toward IoT and connected-device development.

---

# 15. Repository Notes

The individual sketches in this archive are reconstructed/documented from the ESP32 DevKit V1 work rather than being presented as byte-for-byte exports of every historical Arduino IDE sketch.

Where exact historical code was not recoverable, the file is marked/documented accordingly instead of pretending that an unverified implementation was the original one.

Before connecting motors or external power:

1. Disconnect the USB cable if the power arrangement could back-feed the board.
2. Verify the motor-driver wiring.
3. Verify common ground.
4. Check motor voltage.
5. Test with the wheels lifted from the ground.
6. Keep an accessible STOP control.

For API projects, replace placeholder credentials with your own private credentials and do not commit secrets to public repositories.

---

# 16. Final Project Summary

The classic ESP32 DevKit V1 work covered several important embedded-development areas:

- Wi-Fi Access Point development
- HTTP WebServer development
- browser-based hardware control
- PWM
- DC motor control
- motor-driver interfacing
- DHT11 sensor acquisition
- MQ135 analog sensing
- WebSocket networking
- JSON serialization
- REST API consumption
- OpenWeather integration
- Bluetooth Classic
- A2DP audio
- USB-UART bridging
- ESP32-CAM programming
- I²S concepts
- digital audio streaming

Together, these projects represent a progression from basic microcontroller GPIO work to networked embedded systems and digital audio.
