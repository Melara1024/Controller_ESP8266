#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

# define PI 3.1415

const char *ssid = "*robot name*";
const char *pass = "*your passward*";
ESP8266WebServer Server(80);

int RIGHT_REAR[] = {5, 14};
int RIGHT_FRONT[] = {2, 4};
int LEFT_REAR[] = {13, 12};
int LEFT_FRONT[] = {16, 15};

int joyX = 0;
int joyY = 0;
int turnL = 0;
int turnR = 0;

// Event Subscriber
void joyEvent() {
  String px = Server.arg("px");
  String py = Server.arg("py");
  if(px != NULL)  joyX = px.toInt();
  if(py != NULL)  joyY = py.toInt();
}
void leftEvent() {
  if(Server.arg("l") != NULL)  turnL = (Server.arg("l")).toInt();
}
void rightEvent() {
  if(Server.arg("r") != NULL)  turnR = (Server.arg("r")).toInt();
}


void motor(int *pin, int power){
  if(abs(power) < 10){
    analogWrite(pin[0],  255);
    analogWrite(pin[1],  255);
  }
  else if(power > 0){
    if (power > 100) power = 100;
    analogWrite(pin[0],  205 + power/2);
    analogWrite(pin[1],  0);
  }
  else{
    analogWrite(pin[0],  0);
    if (power < 100) power = -100;
    analogWrite(pin[1],  205 - power/2);
  }
}


void handleRoot() {
  Server.sendHeader("Location", "/index.html", true);  //Redirect to our html web page
  Server.send(302, "text/plane", "");
}
void handleWebRequests() {
  if (loadFromSpiffs(Server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += Server.uri();
  message += "\nMethod: ";
  message += (Server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += Server.args();
  message += "\n";
  for (uint8_t i = 0; i < Server.args(); i++) {
    message += " NAME:" + Server.argName(i) + "\n VALUE:" + Server.arg(i) + "\n";
  }
  Server.send(404, "text/plain", message);
}
void setup() {
  SPIFFS.begin();

  delay(100);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);

  WiFi.softAPIP();

  Server.on("/", handleRoot);
  Server.on("/joyEvent", joyEvent);
  Server.on("/leftEvent", leftEvent);
  Server.on("/rightEvent", rightEvent);
  Server.onNotFound(handleWebRequests);
  Server.begin();

  delay(100);

  pinMode(RIGHT_REAR[0], OUTPUT);
  pinMode(RIGHT_REAR[1], OUTPUT);
  pinMode(RIGHT_FRONT[0], OUTPUT);
  pinMode(RIGHT_FRONT[1], OUTPUT);
  pinMode(LEFT_REAR[0], OUTPUT);
  pinMode(LEFT_REAR[1], OUTPUT);
  pinMode(LEFT_FRONT[0], OUTPUT);
  pinMode(LEFT_FRONT[1], OUTPUT);
}

void loop() {
  Server.handleClient();
  if (turnL) {
    motor(RIGHT_REAR, -100);
    motor(RIGHT_FRONT, -100);
    motor(LEFT_REAR, -100);
    motor(LEFT_FRONT, -100);
  }
  else if (turnR) {
    motor(RIGHT_REAR, 100);
    motor(RIGHT_FRONT, 100);
    motor(LEFT_REAR, 100);
    motor(LEFT_FRONT, 100);
  }
  else{
    int theta = atan2(joyY, joyX) * 180.0 / PI;

    if (abs(joyX) + abs(joyY) < 10) {
      motor(RIGHT_REAR, 0);
      motor(RIGHT_FRONT, 0);
      motor(LEFT_REAR, 0);
      motor(LEFT_FRONT, 0);
    }
    else if (theta <= 22.5 && theta > -22.5) {
      // ↑
      motor(RIGHT_REAR, 255);
      motor(RIGHT_FRONT, 255);
      motor(LEFT_REAR, -255);
      motor(LEFT_FRONT, -255);
    }
    else if ((theta <= -157.5 && theta > -180) || (theta <= 180 && theta > 157.5)) {
      // ↓
      motor(RIGHT_REAR, -255);
      motor(RIGHT_FRONT, -255);
      motor(LEFT_REAR, 255);
      motor(LEFT_FRONT, 255);
    }
    else if (theta <= 112.8 && theta > 67.5) {
      // →
      motor(RIGHT_REAR, 255);
      motor(RIGHT_FRONT, -255);
      motor(LEFT_REAR, 255);
      motor(LEFT_FRONT, -255);
    }
    else if (theta <= -67.5 && theta > -112.5) {
      // ←
      motor(RIGHT_REAR, -255);
      motor(RIGHT_FRONT, 255);
      motor(LEFT_REAR, -255);
      motor(LEFT_FRONT, 255);
    }
    else if (theta <= 67.5 && theta > 22.5) {
      // ↗
      motor(RIGHT_REAR, 0);
      motor(RIGHT_FRONT, 255);
      motor(LEFT_REAR, -255);
      motor(LEFT_FRONT, 0);
    }
    else if (theta <= 157.5 && theta > 112.5) {
      // ↘
      motor(RIGHT_REAR, -255);
      motor(RIGHT_FRONT, 0);
      motor(LEFT_REAR, 0);
      motor(LEFT_FRONT, 255);
    }
    else if (theta <= -22.5 && theta > -67.5) {
      // ↖
      motor(RIGHT_REAR, 255);
      motor(RIGHT_FRONT, 0);
      motor(LEFT_REAR, 0);
      motor(LEFT_FRONT, -255);
    }
    else if (theta <= -112.5 && theta > -157.5) {
      // ↙
      motor(RIGHT_REAR, 0);
      motor(RIGHT_FRONT, -255);
      motor(LEFT_REAR, 255);
      motor(LEFT_FRONT, 0);
    }
  }
}

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.html";
  if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (Server.streamFile(dataFile, dataType) != dataFile.size()) {
  }
  dataFile.close();
  return true;
}
