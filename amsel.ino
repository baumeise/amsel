#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>

#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"

#define TX 1
#define RX 3
#define CONTROL_TIMEOUT 1000

char* ssid;
char* pass;

char AP_ssid[] = "Amsel";
char AP_pass[] = "passwort";

char UDP_ip[] = "192.168.4.1";
int UDP_port = 8080;

// Amsel
long lastControlUpdate = 0;
float drive_factor = 0.0;
float steer_factor = 0.0;

// DC-Motor 1
const int full_speed1 = 1024;
const int GSM1 = D0;
const int in1 = D8;
const int in2 = D7;

// DC-Motor 2
const int full_speed2 = 1024;
const int GSM2 = D3;
const int in3 = D6;
const int in4 = D5;

// Ultraschall
const int trigPin = RX;
const int echoPin = D4;
long duration;
int distance;

// Display
SSD1306Wire  display(0x3c, D2, D1);
OLEDDisplayUi ui     ( &display );
String userText = "";

// OTA
bool isOtaActive = false;

ESP8266WebServer server(80);
MDNSResponder MDNS;

void stop(bool inLoop=false) {
  drive_factor = 0;
  steer_factor = 0;
  updateWheelSpeed(inLoop);
  server.send(200, "text/plain", "Amsel now stopping!");
}

void setup() {
  //Swap a TX/RX pin to a GPIO.
  // pinMode(TX, FUNCTION_3);
  pinMode(RX, FUNCTION_3);
  
  // Init serial, wait for port
  Serial.begin(115200);
  while (!Serial) { ; }

  // Setup network
  setup_tcp();
  setup_udp();

  // Setup display
  setup_display();
  
  // Start file system
  SPIFFS.begin();

  //Enable OTA Flashing
  startOTA();

  // Set PinMode
  pinMode(GSM1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(GSM2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  analogWrite(GSM1, 0); //Pwm duty cycle 0%  
  analogWrite(GSM2, 0); //Pwm duty cycle 0%  

  // Handle routing
  routes();
}

void loop() {
  loop_display();
  loop_udp();
  loop_tcp();

  // Alive check
  if (lastControlUpdate != 0 && ((millis() - lastControlUpdate) > CONTROL_TIMEOUT)) {
    lastControlUpdate = 0;
    stop();
  }
}
