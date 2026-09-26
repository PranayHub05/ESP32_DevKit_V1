/*
  ESP32 DevKit V1 - Bluetooth Classic A2DP Test

  This is a minimal starting point for testing A2DP output.

  IMPORTANT:
    The API differs slightly between versions of the ESP32-A2DP
    library. If compilation errors occur, compare the installed
    library's examples with this sketch.
*/

#include "BluetoothA2DPSource.h"

BluetoothA2DPSource a2dp_source;

int32_t get_audio_data(Channels *channels, int32_t sample_rate) {
  // Generate silence.
  // This function exists to establish a valid PCM source.
  // Replace it with real PCM audio for actual music/audio streaming.

  static int16_t silence[2] = {0, 0};

  channels->left = silence[0];
  channels->right = silence[1];

  return 0;
}

void setup() {
  Serial.begin(115200);

  Serial.println("Starting ESP32 Bluetooth Classic A2DP source...");
  Serial.println("Select your Bluetooth speaker as the target device.");

  // Replace this with the Bluetooth name of your speaker.
  a2dp_source.start("YOUR_BLUETOOTH_SPEAKER", get_audio_data);
}

void loop() {
  delay(1000);
}
