// Routing
void routes() {
  server.on("/", openHome);
  server.on("/setup", openSetup);
  server.on("/login", handleLoginAttempt);
  server.on("/connect", handleConnect);

  server.on("/forward", []() { 
    handleForward(handleSpecificArg());
  });

  server.on("/reverse", []() {
    handleReverse(handleSpecificArg());
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
  
  server.on("/sensor", []() {
    handleSensor();
  });

  server.onNotFound(handleNotFound);
}
