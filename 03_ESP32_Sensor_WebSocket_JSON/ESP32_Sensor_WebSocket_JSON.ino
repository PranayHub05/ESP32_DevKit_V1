/*
  ESP32 DevKit V1
  DHT11 + MQ135 -> WebSocket JSON

  WebSocket server:
    Port 81

  IMPORTANT:
    Set DHT_PIN and MQ135_PIN to the GPIOs used by your actual wiring.
*/

#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define DHT_TYPE DHT11

const int DHT_PIN = 4;       // CHANGE to your wiring
const int MQ135_PIN = 34;    // CHANGE to your wiring

DHT dht(DHT_PIN, DHT_TYPE);
WebSocketsServer webSocket = WebSocketsServer(81);

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 2000;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.printf("WebSocket client %u connected\n", num);
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("WebSocket client %u disconnected\n", num);
  }
}

void sendSensorJSON() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int mq135 = analogRead(MQ135_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 read failed.");
    return;
  }

  StaticJsonDocument<256> doc;

  doc["device"] = "ESP32";
  doc["id"] = "ESP32-WROOM-01";
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["mq135"] = mq135;

  String output;
  serializeJson(doc, output);

  Serial.println(output);
  webSocket.broadcastTXT(output);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("WebSocket server started on port 81.");
}

void loop() {
  webSocket.loop();

  if (millis() - lastSend >= SEND_INTERVAL) {
    lastSend = millis();
    sendSensorJSON();
  }
}
