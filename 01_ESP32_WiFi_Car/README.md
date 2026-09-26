# ESP32 Wi-Fi Controlled Mini Car

Status: TESTED / DEVELOPED

## Hardware
- ESP32 DevKit V1 / ESP32-WROOM-32
- HW095 motor driver
- 2 geared TT motors
- Battery pack
- Chassis
- Phone/tablet with Wi-Fi

## Wi-Fi
SSID: `ESP32-CAR`
Password: `12345678`
Typical AP address: `192.168.4.1`

## GPIO
- GPIO25
- GPIO26
- GPIO27
- GPIO14

## How it works

The ESP32 creates a Wi-Fi access point and serves an HTML control page. The phone connects directly to the ESP32 and sends HTTP commands.

The controls are intentionally momentary. A heartbeat/timeout mechanism causes the car to stop when commands stop arriving.

## Upload/use

1. Select an ESP32 Dev Module board in Arduino IDE.
2. Install the ESP32 board package.
3. Upload the sketch.
4. Connect to `ESP32-CAR`.
5. Open `192.168.4.1`.
6. Test with wheels lifted.
7. Verify direction before driving.

## Safety

Never power the motors directly from ESP32 GPIO pins. Use the motor driver and an appropriate motor supply.
