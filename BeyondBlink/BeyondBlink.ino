#include "credentials.h"
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

#define LED_BUILTIN 4
bool LEDstatus = LOW;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
 
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  //connect to your local wi-fi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("Web Server IP Address: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  server.handleClient();
}

void print_GPIO_Status(int gpio){
  Serial.print("GPIO ");
  Serial.print(gpio);
  Serial.print(" Status: ");
  if (digitalRead(gpio))
    {Serial.println("ON");}
  else
    {Serial.println("OFF");}
}

void handle_OnConnect() {
  print_GPIO_Status(LED_BUILTIN);
  server.send(200, "text/html", SendHTML(digitalRead(LED_BUILTIN))); 
}

void handle_ledon() {
  digitalWrite(LED_BUILTIN, HIGH);
  print_GPIO_Status(LED_BUILTIN);
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() {
  digitalWrite(LED_BUILTIN, LOW);
  print_GPIO_Status(LED_BUILTIN);
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Controller</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
    ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";}
  else
  {ptr +="<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}