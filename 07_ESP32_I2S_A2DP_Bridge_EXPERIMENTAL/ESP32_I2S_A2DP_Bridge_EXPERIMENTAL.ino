/*
  EXPERIMENTAL ONLY
  ESP32 DevKit V1 - I2S PCM input -> Bluetooth Classic A2DP

  Proposed input wiring:
    DATA  -> GPIO34
    BCLK  -> GPIO25
    WS    -> GPIO26
    GND   -> GND

  IMPORTANT:
    This is an architecture/prototyping skeleton, NOT a confirmed
    finished implementation.

  You must match the I2S source format to the A2DP library's expected
  PCM format and implement buffering/conversion as required.
*/

#include <Arduino.h>

const int I2S_DATA_PIN = 34;
const int I2S_BCLK_PIN = 25;
const int I2S_WS_PIN   = 26;

void setup() {
  Serial.begin(115200);

  Serial.println("Experimental I2S -> A2DP bridge");
  Serial.println("This sketch is intentionally a prototype skeleton.");
  Serial.printf("I2S DATA: GPIO%d\n", I2S_DATA_PIN);
  Serial.printf("I2S BCLK: GPIO%d\n", I2S_BCLK_PIN);
  Serial.printf("I2S WS:   GPIO%d\n", I2S_WS_PIN);

  /*
    Next implementation stages:

    1. Configure ESP32 I2S RX.
    2. Verify sample rate and sample width.
    3. Read PCM blocks.
    4. Buffer PCM safely.
    5. Convert format if required.
    6. Feed the PCM blocks into the A2DP source callback.
    7. Test latency and underrun handling.
  */
}

void loop() {
  delay(1000);
}
