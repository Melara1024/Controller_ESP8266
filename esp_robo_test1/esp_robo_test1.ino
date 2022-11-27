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


void handlePWM(){
  //pwmに格納されている値を持ってくる
  //持ってきたいのはx座標とy座標，回転が押されているかどうか
  String PWM = Server.arg("pwm");
  int p = (((PWM.toInt())*255)/100);
  Serial.println(p);
  analogWrite(RIGHT_REAR_1,p);
  analogWrite(RIGHT_REAR_2,0);

  analogWrite(RIGHT_FRONT_1,p);
  analogWrite(RIGHT_FRONT_2,0);

  analogWrite(LEFT_REAR_1,0);
  analogWrite(LEFT_REAR_2,p);

  analogWrite(LEFT_FRONT_1,0);
  analogWrite(LEFT_FRONT_2,p);
  Server.send(200, "text/plane","");
}

void handleRoot(){
  Server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  Server.send(302, "text/plane","");
}

void handleWebRequests(){
  if(loadFromSpiffs(Server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += Server.uri();
  message += "\nMethod: ";
  message += (Server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += Server.args();
  message += "\n";
  for (uint8_t i=0; i<Server.args(); i++){
    message += " NAME:"+Server.argName(i) + "\n VALUE:" + Server.arg(i) + "\n";
  }
  Server.send(404, "text/plain", message);
  Serial.println(message);
}

void setup() {

  SPIFFS.begin();

  if(SERIAL) Serial.begin(115200);
  
  delay(100);
  if(SERIAL) Serial.println("\n*** Dongbeino ***");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  
  if(SERIAL)
  {
    Serial.print("network: "); Serial.println(ssid);
    Serial.print("address: "); Serial.println(WiFi.softAPIP());
  }
  WiFi.softAPIP();

  Server.on("/",handleRoot);
  Server.on("/setLED",handlePWM);
  Server.onNotFound(handleWebRequests);
  Server.begin();  
}

void loop() {
 Server.handleClient();
}

bool loadFromSpiffs(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (Server.hasArg("download")) dataType = "application/octet-stream";
  if (Server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
