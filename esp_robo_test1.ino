#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#define RIGHT_REAR_1 5
#define RIGHT_REAR_2 14

#define RIGHT_FRONT_1 2
#define RIGHT_FRONT_2 4

#define LEFT_REAR_1 13
#define LEFT_REAR_2 12

#define LEFT_FRONT_1 16
#define LEFT_FRONT_2 15

#define SERIAL false

const char *ssid = "dongbeino1";
const char *pass = "topsecret";      //  ８文字以上
ESP8266WebServer Server(80);         //  ポート番号（HTTP）
int Counter = 0;                     //  お客さんカウンタ

void handlePWM(){
  String PWM = server.arg("pwm");
  int p = 1024 - (PWM.toInt())*10;
  Serial.println(p);
  analogWrite(LED,p);
  server.send(200, "text/plane","");
}

void handleRoot(){
  server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  server.send(302, "text/plane","");
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

//  Server.on(...) を指定せず，すべてを handleNotFound で処理する．
//  URI で指定されるファイルがあればクライアントに転送する．
//  なければ 404 エラー！
void handleNotFound() {
  if (! handleFileRead(Server.uri())) {
    //  ファイルが見つかりません
    if(SERIAL) Serial.println("404 not found");
    Server.send(404, "text/plain", "File not found in Dongbeino...");
  }
}

//  SPIFSS のファイルをクライアントに転送する
bool handleFileRead(String path) {
  if(SERIAL) Serial.println("handleFileRead: trying to read " + path);
  // パス指定されたファイルがあればクライアントに送信する
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);

  if(SERIAL) Serial.println("searching index.html");
  if (SPIFFS.exists(path)) {
    if(SERIAL) Serial.println("handleFileRead: sending " + path);
    File file = SPIFFS.open(path, "r");
    Server.streamFile(file, contentType);
    file.close();
    if(SERIAL) Serial.println("handleFileRead: sent " + path);
    return true;
  }
  else {
    if(SERIAL) Serial.println("handleFileRead: 404 not found");
    Server.send (404, "text/plain", "ESP: 404 not found");
    return false;
  }
}


void setup() {
  //  ファイルシステム
  SPIFFS.begin();
  //  シリアルモニタ（動作ログ）
  
  if(SERIAL) Serial.begin(115200);               //  ESP 標準の通信速度 115200
  
  delay(100);                         //  100ms ほど待ってからログ出力可
  if(SERIAL) Serial.println("\n*** Dongbeino ***");
  //  アクセスポイントの構成
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  
  if(SERIAL)
  {
    Serial.print("network: "); Serial.println(ssid);
    Serial.print("address: "); Serial.println(WiFi.softAPIP());
  }
  WiFi.softAPIP();
  //Initialize Webserver
  Server.on("/",handleRoot);
  Server.on("/setLED",handlePWM); //Reads ADC function is called from out index.html
  Server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
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
