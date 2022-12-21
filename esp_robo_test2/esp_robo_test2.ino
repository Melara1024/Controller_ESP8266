#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char* htmlfile = "/index.html";

#define RIGHT_REAR_1 5
#define RIGHT_REAR_2 14

#define RIGHT_FRONT_1 2
#define RIGHT_FRONT_2 4

#define LEFT_REAR_1 13
#define LEFT_REAR_2 12

#define LEFT_FRONT_1 16
#define LEFT_FRONT_2 15

#define SERIAL true

const char *ssid = "access_point";
const char *pass = "pasuwaado";
ESP8266WebServer Server(80);

// x方向の原点
int originX = 0;
// y方向の原点
int originY = 0;
// x方向の速度
int joyX = 0;
// y方向の速度
int joyY = 0;
// 左旋回ボタン
int turnL = 0;
// 右旋回ボタン
int turnR = 0;

void handlePWM() {
  if(Server.arg("ox") != NULL)  originX = (Server.arg("ox")).toInt();
  if(Server.arg("oy") != NULL)  originY = (Server.arg("oy")).toInt();
  if(Server.arg("px") != NULL)  joyX = (Server.arg("px")).toInt();
  if(Server.arg("py") != NULL)  joyY = (Server.arg("py")).toInt();
  if(Server.arg("l") != NULL)  turnL = (Server.arg("l")).toInt();
  if(Server.arg("r") != NULL)  turnR = (Server.arg("r")).toInt();

  // 原点設定時にはロボットを動かさない(誤動作するので)
  if(Server.arg("ox") != NULL || Server.arg("oy") != NULL) return;
  
  if (turnL) {
    turnRobot(true);
  }
  else if (turnR) {
    turnRobot(false);
  }
  else {
    moveRobot(joyX - originX, joyY - originY);
  }
  Server.send(200, "text/plane", "");
}

void moveRobot(int x, int y) {
  // 最初にタッチした位置を原点とする位置ベクトルが格納されている
  Serial.printf("relX = %d\n", x);
  Serial.printf("relY = %d\n", y);

  //Todo xとyの値をモーターの出力値に変換する必要がある
  
//  analogWrite(RIGHT_REAR_1, 70);
//  analogWrite(RIGHT_REAR_2, 0);
//
//  analogWrite(RIGHT_FRONT_1, 70);
//  analogWrite(RIGHT_FRONT_2, 0);
//
//  analogWrite(LEFT_REAR_1, 70);
//  analogWrite(LEFT_REAR_2, p);
//
//  analogWrite(LEFT_FRONT_1, 70);
//  analogWrite(LEFT_FRONT_2, p);

}
void turnRobot(boolean left) {
  Serial.printf("left = %d\n", left);

  //Todo left値のtrue/falseにあわせて定速回転するだけ
  
//  analogWrite(RIGHT_REAR_1, p);
//  analogWrite(RIGHT_REAR_2, 0);
//
//  analogWrite(RIGHT_FRONT_1, p);
//  analogWrite(RIGHT_FRONT_2, 0);
//
//  analogWrite(LEFT_REAR_1, 0);
//  analogWrite(LEFT_REAR_2, p);
//
//  analogWrite(LEFT_FRONT_1, 0);
//  analogWrite(LEFT_FRONT_2, p);
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
  Serial.println(message);
}

void setup() {

  SPIFFS.begin();

  if (SERIAL) Serial.begin(115200);

  delay(100);
  if (SERIAL) Serial.println("\n*** Dongbeino ***");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);

  if (SERIAL)
  {
    Serial.print("network: "); Serial.println(ssid);
    Serial.print("address: "); Serial.println(WiFi.softAPIP());
  }
  WiFi.softAPIP();

  Server.on("/", handleRoot);
  Server.on("/setLED", handlePWM);
  Server.onNotFound(handleWebRequests);
  Server.begin();
}

void loop() {
  Server.handleClient();
}

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (Server.hasArg("download")) dataType = "application/octet-stream";
  if (Server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
