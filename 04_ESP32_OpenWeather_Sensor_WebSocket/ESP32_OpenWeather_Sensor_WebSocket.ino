/*
  ESP32 DevKit V1
  DHT11 + MQ135 + OpenWeather + WebSocket

  WebSocket port: 81

  Replace the configuration values below.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char* OPENWEATHER_API_KEY = "YOUR_OPENWEATHER_API_KEY";
const char* CITY = "Kolkata";
const char* COUNTRY_CODE = "IN";

const int DHT_PIN = 4;       // CHANGE to your wiring
const int MQ135_PIN = 34;    // CHANGE to your wiring

#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
WebSocketsServer webSocket = WebSocketsServer(81);

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 10000;

float weatherTemp = NAN;
float weatherHumidity = NAN;
String weatherDescription = "";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.printf("Client %u connected\n", num);
  }
}

bool getWeather() {
  HTTPClient http;

  String url =
      "http://api.openweathermap.org/data/2.5/weather?q=" +
      String(CITY) + "," + String(COUNTRY_CODE) +
      "&appid=" + String(OPENWEATHER_API_KEY) +
      "&units=metric";

  http.begin(url);

  int code = http.GET();

  if (code <= 0) {
    Serial.printf("HTTP error: %d\n", code);
    http.end();
    return false;
  }

  if (code != 200) {
    Serial.printf("OpenWeather HTTP status: %d\n", code);
    http.end();
    return false;
  }

  String payload = http.getString();

  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("Weather JSON error: ");
    Serial.println(error.c_str());
    http.end();
    return false;
  }

  weatherTemp = doc["main"]["temp"] | NAN;
  weatherHumidity = doc["main"]["humidity"] | NAN;
  weatherDescription = doc["weather"][0]["description"].as<String>();

  http.end();
  return true;
}

void sendCombinedJSON() {
  float localTemp = dht.readTemperature();
  float localHumidity = dht.readHumidity();
  int mq135 = analogRead(MQ135_PIN);

  if (isnan(localTemp) || isnan(localHumidity)) {
    Serial.println("DHT11 read failed.");
    return;
  }

  StaticJsonDocument<512> doc;

  JsonObject device = doc.createNestedObject("device");
  device["name"] = "ESP32";
  device["id"] = "ESP32-WROOM-01";

  JsonObject sensor = doc.createNestedObject("sensor");
  sensor["temperature"] = localTemp;
  sensor["humidity"] = localHumidity;
  sensor["mq135"] = mq135;

  JsonObject weather = doc.createNestedObject("weather");
  weather["temperature"] = weatherTemp;
  weather["humidity"] = weatherHumidity;
  weather["description"] = weatherDescription;

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

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("WebSocket server: port 81");
}

void loop() {
  webSocket.loop();

  if (millis() - lastSend >= SEND_INTERVAL) {
    lastSend = millis();

    if (getWeather()) {
      Serial.println("Weather updated.");
    } else {
      Serial.println("Weather update failed; local data will still be sent.");
    }

    sendCombinedJSON();
  }
}
