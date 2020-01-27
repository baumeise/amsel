// Routing
void routes() {
  // Render 
  server.on("/", openHome);
  server.on("/setup", openSetup);

  // Connection
  server.on("/login", handleLoginAttempt);
  server.on("/connect", handleConnect);

  // Setters
  server.on("/calibrate", handleCalibration);

  // Getters
  server.on("/distance", handleDistance);

  // Skills
  server.on("/forward", []() { 
    handleForward(handleSpecificArg());
  });

  server.on("/backward", []() {
    handleBackward(handleSpecificArg());
  });

  server.on("/stop", []() {
    stop();
  });

  server.on("/right", []() {
    handleRight(handleSpecificArg());
  });

  server.on("/left", []() {
    handleLeft(handleSpecificArg());
  });

  server.on("/print", []() {
    handlePrint(handleStringArg());
  });

  // Helpers
  server.onNotFound(handleNotFound);
}
