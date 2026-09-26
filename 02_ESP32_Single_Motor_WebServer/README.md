# ESP32 Single-Motor WebServer Experiment

Status: TESTED

## Purpose

A smaller motor-control experiment for testing one motor and PWM/WebServer control independently of the complete car.

## Configuration

- SSID: `ESP32-MOTOR`
- AP IP: `192.168.4.1`
- GPIO25: ENA/PWM
- GPIO26: IN1
- GPIO27: IN2
- PWM cap used in the documented experiment: 106/255

The motor must be connected through the motor driver.
