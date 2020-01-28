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
      
      //Start the DNS Server 
      if (MDNS.begin("amsel", WiFi.localIP())) {
      Serial.println("DNS Server Amsel started!"); 
      }
      MDNS.addService("http","tcp",80);
      
      Serial.print("Now visit ");
      Serial.print("amsel.local or ");
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

IPAddress NetworkIP() {
  return WiFi.localIP();
}

void setup_tcp() {
  // Init access point
  riseAP();

  // Try to connect to local network
  connectToWifi();
}

void loop_tcp() {
  server.handleClient();
  MDNS.update();
  ArduinoOTA.handle();
}

//Enable OTA Flashing
void startOTA() {

  ArduinoOTA.setHostname("amsel");

  ArduinoOTA.onStart([]() {
    isOtaActive = true;
    
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    Serial.println("Start updating " + type);
    drawOtaStart();
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    drawOtaEnd();
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    drawOtaProgress(progress, total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
      drawOtaError("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
      drawOtaError("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
      drawOtaError("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
      drawOtaError("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
      drawOtaError("End Failed");
    }
  });

  ArduinoOTA.begin();
}
