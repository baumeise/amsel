 void riseAP() {
  WiFi.softAP(AP_ssid, AP_pass);
  server.begin();

  serialWelcome();
  serialWelcomeInstructions();
}

void connectToWifi() {

  if (ssid and pass) {
    Serial.println();
    WiFi.begin(ssid, pass);

    Serial.println();
    Serial.print("Attempting to connect to: ");
    Serial.print(ssid);

    // Wait for the Wi-Fi to connect
    int pending = 0;
    while (WiFi.status() != WL_CONNECTED and pending < 20) { 
      delay(1000);
      Serial.print('.');
      pending++;
    }

    // Log the connection status
    if (WiFi.status() == WL_CONNECTED) {
      
      Serial.println();
      Serial.print("Connected to: ");
      Serial.print(ssid);
      
      Serial.println();
      Serial.println();
      Serial.print("Disconnect from \"Amsel\" and connect to ");
      Serial.println(ssid);
      Serial.print("Then visit ");
      Serial.print(WiFi.localIP());
      Serial.print(" to proceed.");

      server.sendHeader("Location", String("/"), true);
      server.send ( 302, "text/plain", "");
      
    } else {

      Serial.println("Connection failed. Try again!");
      server.sendHeader("Location", String("/setup"), true);
      server.send ( 302, "text/plain", "");
      
    }

  } else {
    
    server.sendHeader("Location", String("/setup"), true);
    server.send ( 302, "text/plain", "");
      
  }

}

IPAddress AccessPointIP() {
  return WiFi.softAPIP();
}
