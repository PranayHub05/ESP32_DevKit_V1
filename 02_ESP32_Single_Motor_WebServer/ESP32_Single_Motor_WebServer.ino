/*
  ESP32 DevKit V1 - Single Motor WebServer Test

  Documented configuration:
    ENA/PWM -> GPIO25
    IN1     -> GPIO26
    IN2     -> GPIO27

  AP:
    ESP32-MOTOR
    192.168.4.1

  The documented experiment capped PWM at 106/255.
*/

#include <WiFi.h>
#include <WebServer.h>

const char* SSID = "ESP32-MOTOR";
const char* PASSWORD = "12345678";

WebServer server(80);

const int ENA = 25;
const int IN1 = 26;
const int IN2 = 27;

const int PWM_CHANNEL = 0;
const int PWM_FREQUENCY = 1000;
const int PWM_RESOLUTION = 8;
const int PWM_LIMIT = 106;

int speedValue = 80;

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  ledcWrite(PWM_CHANNEL, 0);
}

void forwardMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(PWM_CHANNEL, speedValue);
}

void reverseMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(PWM_CHANNEL, speedValue);
}

String page() {
  String html = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESP32 Motor</title>
<style>
body { font-family:Arial; text-align:center; background:#111; color:white; }
button { width:110px; height:65px; margin:8px; font-size:24px; }
input { width:80%; }
</style>
</head>
<body>
<h1>ESP32 MOTOR</h1>

<input id="speed" type="range" min="0" max="106" value="80"
       oninput="setSpeed(this.value)">

<br><br>

<button onclick="cmd('forward')">▲</button><br>
<button onclick="cmd('reverse')">▼</button>
<button onclick="cmd('stop')">STOP</button>

<script>
function cmd(x) {
  fetch('/' + x).catch(()=>{});
}
function setSpeed(x) {
  fetch('/speed?value=' + x).catch(()=>{});
}
</script>
</body>
</html>
)HTML";

  return html;
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(ENA, PWM_CHANNEL);

  stopMotor();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);

  server.on("/", []() {
    server.send(200, "text/html", page());
  });

  server.on("/forward", []() {
    forwardMotor();
    server.send(200, "text/plain", "FORWARD");
  });

  server.on("/reverse", []() {
    reverseMotor();
    server.send(200, "text/plain", "REVERSE");
  });

  server.on("/stop", []() {
    stopMotor();
    server.send(200, "text/plain", "STOP");
  });

  server.on("/speed", []() {
    if (server.hasArg("value")) {
      speedValue = constrain(server.arg("value").toInt(), 0, PWM_LIMIT);
    }
    server.send(200, "text/plain", String(speedValue));
  });

  server.begin();

  Serial.println("ESP32 MOTOR AP started.");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  server.handleClient();
}
