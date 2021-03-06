// Render
void openHome() {
  File file = SPIFFS.open("/home.html", "r"); 
  size_t sent = server.streamFile(file, "text/html");
  file.close();
}

void openSetup() {
  File file = SPIFFS.open("/setup.html", "r"); 
  size_t sent = server.streamFile(file, "text/html");
  file.close();
}

void openConnected() {
  File file = SPIFFS.open("/connected.html", "r"); 
  size_t sent = server.streamFile(file, "text/html");
  file.close();
}

// Connection
void handleLoginAttempt() {
  server.sendHeader("Location", "/connect", true);
  server.send( 302, "text/plain", "");
  
  String ssid_str = server.arg("ssid");
  int ssid_len = ssid_str.length() + 1;
  char ssid_arr[ssid_len];

  String pass_str = server.arg("pass");
  int pass_len = pass_str.length() + 1;
  char pass_arr[pass_len];
  
  ssid_str.toCharArray(ssid_arr, ssid_len);
  pass_str.toCharArray(pass_arr, pass_len);
  ssid = ssid_arr;
  pass = pass_arr;

  connectToWifi();
}

void handleConnect() {
  if( WiFi.status() == WL_CONNECTED ) {
    openConnected();
  } else {
    server.send(200, "text/plain", "Connection failed ...");  
  } 
}

// Skills
void handleForward(int speed, bool inLoop=false) {
  drive_factor = (float)speed/100.0;
  updateWheelSpeed(inLoop);
  server.send(200, "text/plain", "Amsel now moving forward!"); 
}

void handleBackward(int speed, bool inLoop=false) {
  drive_factor = -(float)speed/100.0;
  updateWheelSpeed(inLoop);
  server.send(200, "text/plain", "Amsel now moving backward!");  
}

void handleLeft(int speed, bool inLoop=false) {
  steer_factor = (float)speed/100.0; 
  updateWheelSpeed(inLoop);
  server.send(200, "text/plain", "Amsel now turning left!");
}

void handleRight(int speed, bool inLoop=false) {
  steer_factor = -(float)speed/100.0; 
  updateWheelSpeed(inLoop);
  server.send(200, "text/plain", "Amsel now turning right!");
}

void updateWheelSpeed(bool inLoop) {
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
  if (inLoop) {
    lastControlUpdate = millis();
  }
}

//Setter
void handleCalibration() {
  String left_str = server.arg("l");
  String right_str = server.arg("r");
  float left_float = left_str.toFloat();
  float right_float = right_str.toFloat();
  
  float full_speed1 = (float)full_speed1*left_float;
  float full_speed2 = (float)full_speed2*right_float;

  server.send(200, "text/plain", String(full_speed1, full_speed2));
}

// Getter
void handleDistance() {
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

// Display
void handlePrint(String text) {
  userText = text;
  server.send(200, "text/plain", "Amsel printed: \"" + text + "\"");
}

// Helper
void handleNotFound() {
  server.send(404, "text/plain", "404 - Not found ...");
}

int handleSpecificArg() { 
  int value = 0;
  // Check for speed argument
  if (server.arg("speed").length() > 0) {
    value = server.arg("speed").toInt();
  } else {
    value = 0;
  }
  return value;
}

String handleStringArg() { 
  String value = "";
  // Check for speed argument
  if (server.arg("string").length() > 0) {
    value = server.arg("string");                          
  } else {
    value = "";                            
  }
  return value;
}
