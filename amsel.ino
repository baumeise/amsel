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

// OTA
bool isOtaActive = false;

ESP8266WebServer server(80);
MDNSResponder MDNS;

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
  if (millis() - lastControlUpdate > CONTROL_TIMEOUT) {
    stop();
  }
}

// Amsel Skills
void handleForward(int speed) {
  drive_factor = (float)speed/100.0;
  updateWheelSpeed();
  server.send(200, "text/plain", "Amsel now moving forward!"); 
}

void handleReverse(int speed) {
  drive_factor = -(float)speed/100.0;
  updateWheelSpeed();
  server.send(200, "text/plain", "Amsel now moving backwards!");  
}

void stop() {
  drive_factor = 0;
  steer_factor = 0;
  updateWheelSpeed();
  server.send(200, "text/plain", "Amsel now stopping!");
}

void handleLeft(int speed) {
  steer_factor = (float)speed/100.0; 
  updateWheelSpeed();
  server.send(200, "text/plain", "Amsel now turning left!");
}

void handleRight(int speed) {
  steer_factor = -(float)speed/100.0; 
  updateWheelSpeed();
  server.send(200, "text/plain", "Amsel now turning right!");
}

void updateWheelSpeed() {
  float wheelSpeedLeft  = (float)full_speed1*(drive_factor-steer_factor);
  float wheelSpeedRight = (float)full_speed2*(drive_factor+steer_factor);
  
  analogWrite(GSM1, abs(wheelSpeedLeft));
  analogWrite(GSM2, abs(wheelSpeedRight));

  // Left wheel direction
  if (wheelSpeedLeft > 0) {
    digitalWrite(in1, LOW);   
    digitalWrite(in2, HIGH);
  } else if (wheelSpeedLeft < 0) {
    digitalWrite(in1, HIGH);   
    digitalWrite(in2, LOW);
  } else { // wheelSpeed == 0
    digitalWrite(in1, LOW);  
    digitalWrite(in2, LOW); 
  }

  // Right wheel direction
  if (wheelSpeedRight > 0) {
    digitalWrite(in3, HIGH);   
    digitalWrite(in4, LOW);
  } else if (wheelSpeedRight < 0) {
    digitalWrite(in3, LOW);   
    digitalWrite(in4, HIGH);
  } else { // wheelSpeed == 0
    digitalWrite(in3, LOW);  
    digitalWrite(in4, LOW); 
  }
  
  lastControlUpdate = millis();
}

void handleSteering() {
  String left_str = server.arg("l");
  String right_str = server.arg("r");
  String speed_str = server.arg("s");
  float left_float = left_str.toFloat();
  float right_float = right_str.toFloat();
  float speed_float = speed_str.toFloat();
  
  if ( speed_float > 0 ) {
    analogWrite(GSM1, full_speed1 * speed_float * right_float);
    analogWrite(GSM2, full_speed2 * speed_float * left_float);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else {
    analogWrite(GSM1, (full_speed1 * speed_float * right_float) * -1);
    analogWrite(GSM2, (full_speed2 * speed_float * left_float) * -1);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200);
}

void handleSensor() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating and sending the distance
  distance = (duration/2) / 29.1; // in cm
  server.send(200, "text/plain", String(distance));
}
