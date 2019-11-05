#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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
char AP_pass[] = "password";

ESP8266WebServer server(80);

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

  // Handle routing
  routes();
}

void loop() { 
  server.handleClient();
}
