#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  </style>
</head>
<body>
  Hi there, I am Amsel!
</body>
<script></script>
</html>)rawliteral";

char* ssid;
char* pass;

char AP_ssid[] = "Amsel";
char AP_pass[] = "passwort";

//DC-Motor 1
int GSM1 = D5;
int in1 = D1;
int in2 = D2;

//DC-Motor 2
int GSM2 = D7;
int in3 = D3;
int in4 = D4;

ESP8266WebServer server(80);
MDNSResponder MDNS;

void setup() {
  // Init serial, wait for port
  Serial.begin(115200);
  while (!Serial) { ; }

  // Init access point
  riseAP();

  // Try to connect to local network
  connectToWifi();
  
  // Start file system
  SPIFFS.begin();

  //Enable OTA Flashing
  startOTA();

  // Set PinMode
  pinMode(GSM1, OUTPUT);
  pinMode(GSM2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Handle routing
  routes();
}

void loop() { 
  server.handleClient();
  MDNS.update();
  ArduinoOTA.handle();
}

// Amsel Skills
void handleForward() {
  digitalWrite(in1, LOW);  
  digitalWrite(in2, HIGH);
  analogWrite(GSM1, 1000);    
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH);
  analogWrite(GSM2, 1000);  
  server.send(200, "text/plain", "Amsel now moving forward!"); 
}

void handleReverse() {
  digitalWrite(in1, HIGH);   
  digitalWrite(in2, LOW);  
  analogWrite(GSM1, 1000);    
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(GSM2, 1000); 
  server.send(200, "text/plain", "Amsel now moving backwards!");  
}

void stop() {
  digitalWrite(in1, LOW);  
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
  server.send(200, "text/plain", "Amsel now stopping!");
}

void handleLeft() {
  digitalWrite(in1, LOW);  
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH);
  analogWrite(GSM2, 1000);   
  delay(1000);
  stop();
   server.send(200, "text/plain", "Amsel now turning left!");
}

void handleRight() {
  digitalWrite(in1, LOW);  
  digitalWrite(in2, HIGH);
  analogWrite(GSM1, 1000);   
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
  delay(1000);
  stop();
  server.send(200, "text/plain", "Amsel now turning right!");
}