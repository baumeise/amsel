// Callbacks
void handleSetup() {
  File file = SPIFFS.open("/setup.html", "r"); 
  size_t sent = server.streamFile(file, "text/html");
  file.close();
}

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
  server.send(200, "text/plain", "Connected");
} else {
  server.send(200, "text/plain", "Connection failed ...");  
} 
}

void handleHome() {
  server.send(200, "text/html", index_html);
}

void handleNotFound() {
  server.send(404, "text/plain", "404 - Not found ...");
}

// Routing
void routes() {
  server.on("/", handleHome);
  server.on("/setup", handleSetup);
  server.on("/login", handleLoginAttempt);
  server.on("/connect", handleConnect);
  server.onNotFound(handleNotFound);  
  server.on("/forward", []() { 
    handleForward();
  });
  server.on("/reverse", []() {
    handleReverse();
  });
  server.on("/stop", []() {
    stop();
  });
  server.on("/right", []() {
    handleRight();
  });
  server.on("/left", []() {
    handleLeft();
  });
}