#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char* htmlfile = "/index.html";

#define SERIAL true

const char *ssid = "access_point";
const char *pass = "pasuwaado";
ESP8266WebServer Server(80);

unsigned long ts;
unsigned long te;

int RIGHT_REAR[] = {5, 14};
int RIGHT_FRONT[] = {2, 4};
int LEFT_REAR[] = {13, 12};
int LEFT_FRONT[] = {16, 15};

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

void joyStart() {
  if(Server.arg("ox") != NULL)  originX = (Server.arg("ox")).toInt();
  if(Server.arg("oy") != NULL)  originY = (Server.arg("oy")).toInt();
}

void joyEvent() {
  if(Server.arg("px") != NULL)  joyX = (Server.arg("px")).toInt() - originX;
  if(Server.arg("py") != NULL)  joyY = (Server.arg("py")).toInt() - originY;
}

void leftEvent() {
  if(Server.arg("l") != NULL)  turnL = (Server.arg("l")).toInt();
}

void rightEvent() {
  if(Server.arg("r") != NULL)  turnR = (Server.arg("r")).toInt();
}

// ピンを配列として読めるようにしたほうが便利
void motor(int *pin, int power){
  if(power > 0){
    analogWrite(pin[0],  power);
    analogWrite(pin[1],  0);
  }
  else{
    analogWrite(pin[0],  0);
    analogWrite(pin[1], -1*power);
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

  //Server.on("/", handleRoot);
  Server.on("/joyStart", joyStart);
  Server.on("/joyEvent", joyEvent);
  Server.on("/leftEvent", leftEvent);
  Server.on("/rightEvent", rightEvent);
  Server.onNotFound(handleWebRequests);
  Server.begin();
}

int t = 0;

void loop() {
  // さすがに細かく分けると重すぎることがわかった
  // 十字+X字配置にして前進・後進・左右・斜め移動を行う

  // 本当はpub-sub通信もどきを実装できればいけるんだと思う
  if(t > 1000) {
    ts = micros();
    Server.handleClient();
    te = micros();
    Serial.printf("%d ms\n", te-ts);
    t = 0;
  }
  
   if(t % 10  == 0){
      if (turnL) {
      motor(RIGHT_REAR, -255);
      motor(RIGHT_FRONT, -255);
      motor(LEFT_REAR, -255);
      motor(LEFT_FRONT, -255);
    }
    else if (turnR) {
      motor(RIGHT_REAR, 255);
      motor(RIGHT_FRONT, 255);
      motor(LEFT_REAR, 255);
      motor(LEFT_FRONT, 255);
    }
    else{
      motor(RIGHT_REAR, (joyX + joyY)/4);
      motor(RIGHT_FRONT, (joyY - joyX)/4);
      motor(LEFT_REAR, (joyX - joyY)/4);
      motor(LEFT_FRONT, (-1*joyX - joyY)/4);
    }
  }
  t++;
}

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";
  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (Server.hasArg("download")) dataType = "application/octet-stream";
  if (Server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
