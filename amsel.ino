#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
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
const int full_speed1 = 1024;
int GSM1 = D1;
int in1 = D8;
int in2 = D7;

//DC-Motor 2
const int full_speed2 = 1024;
int GSM2 = D2;
int in3 = D5;
int in4 = D6;

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

  analogWrite(GSM1, full_speed1); //Pwm duty cycle 100%  
  analogWrite(GSM2, full_speed2); //Pwm duty cycle 100%  
  
  // Handle routing
  routes();
}

void loop() { 
  server.handleClient();
  MDNS.update();
  ArduinoOTA.handle();
}

// Amsel Skills
void handleForward(long speed) {
  analogWrite(GSM1, (full_speed1*speed)/100);
  analogWrite(GSM2, (full_speed2*speed)/100);
  digitalWrite(in1, LOW);  
  digitalWrite(in2, HIGH);   
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH);
  server.send(200, "text/plain", "Amsel now moving forward!"); 
}

void handleReverse(long speed) {
  analogWrite(GSM1, (full_speed1*speed)/100);
  analogWrite(GSM2, (full_speed2*speed)/100);
  digitalWrite(in1, HIGH);   
  digitalWrite(in2, LOW);     
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
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
  analogWrite(GSM2, full_speed2);
  digitalWrite(in1, LOW);  
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH);   
  delay(1000);
  stop();
   server.send(200, "text/plain", "Amsel now turning left!");
}

void handleRight() {
  analogWrite(GSM1, full_speed1);
  digitalWrite(in1, LOW);  
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
  delay(1000);
  stop();
  server.send(200, "text/plain", "Amsel now turning right!");
}
