/*
  ESP32 DevKit V1 - Wi-Fi Controlled Mini Car

  Documented configuration:
    SSID: ESP32-CAR
    Password: 12345678
    AP IP: 192.168.4.1

  HW095 / motor-control pins:
    GPIO25, GPIO26, GPIO27, GPIO14

  IMPORTANT:
    Pin-to-driver labeling can differ between driver-board variants.
    Verify the physical wiring before applying motor power.
*/

#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "ESP32-CAR";
const char* AP_PASSWORD = "12345678";

WebServer server(80);

// Documented car control pins.
const int PIN_A = 25;
const int PIN_B = 26;
const int PIN_C = 27;
const int PIN_D = 14;

// If no movement command is received within this time,
// stop the car. This is a simple communication failsafe.
const unsigned long COMMAND_TIMEOUT = 350;

unsigned long lastCommandTime = 0;

void stopCar() {
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
  digitalWrite(PIN_A, LOW);
}

void forwardCar() {
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, HIGH);
  digitalWrite(PIN_A, LOW);
}

void backwardCar() {
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, LOW);
  digitalWrite(PIN_A, HIGH);
}

void leftCar() {
  // Pivot/turn implementation.
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, HIGH);
  digitalWrite(PIN_A, LOW);
}

void rightCar() {
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
  digitalWrite(PIN_A, HIGH);
}

void acknowledgeCommand() {
  lastCommandTime = millis();
  server.send(200, "text/plain", "OK");
}

const char MAIN_PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Car</title>
<style>
body {
  font-family: Arial, sans-serif;
  text-align: center;
  background: #111;
  color: white;
}
button {
  width: 110px;
  height: 70px;
  margin: 8px;
  font-size: 28px;
  border-radius: 15px;
  border: none;
  touch-action: manipulation;
}
.stop { background: #d33; color: white; }
</style>
</head>
<body>
<h1>ESP32 CAR</h1>

<div>
  <button
    onpointerdown="start('forward')"
    onpointerup="stop()"
    onpointercancel="stop()">▲</button>
</div>

<div>
  <button
    onpointerdown="start('left')"
    onpointerup="stop()"
    onpointercancel="stop()">◀</button>

  <button class="stop" onclick="stop()">STOP</button>

  <button
    onpointerdown="start('right')"
    onpointerup="stop()"
    onpointercancel="stop()">▶</button>
</div>

<div>
  <button
    onpointerdown="start('backward')"
    onpointerup="stop()"
    onpointercancel="stop()">▼</button>
</div>

<script>
let timer = null;

function sendCommand(command) {
  fetch('/' + command).catch(() => {});
}

function start(command) {
  sendCommand(command);

  clearInterval(timer);
  timer = setInterval(() => {
    sendCommand(command);
  }, 150);
}

function stop() {
  clearInterval(timer);
  timer = null;
  sendCommand('stop');
}
</script>
</body>
</html>
)HTML";

void setup() {
  Serial.begin(115200);

  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);

  stopCar();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println();
  Serial.println("ESP32 Car AP started.");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", MAIN_PAGE);
  });

  server.on("/forward", []() {
    forwardCar();
    acknowledgeCommand();
  });

  server.on("/backward", []() {
    backwardCar();
    acknowledgeCommand();
  });

  server.on("/left", []() {
    leftCar();
    acknowledgeCommand();
  });

  server.on("/right", []() {
    rightCar();
    acknowledgeCommand();
  });

  server.on("/stop", []() {
    stopCar();
    acknowledgeCommand();
  });

  server.begin();
  lastCommandTime = millis();
}

void loop() {
  server.handleClient();

  if (millis() - lastCommandTime > COMMAND_TIMEOUT) {
    stopCar();
  }
}
