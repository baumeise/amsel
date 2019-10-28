void serialWelcome() {
    Serial.println();
    Serial.println();
    Serial.println("    _                               _ ");
    Serial.println("   / \\     _ __ ___    ___    ___  | |");
    Serial.println("  / _ \\   | '_ ` _ \\  / __|  / _ \\ | |");
    Serial.println(" / ___ \\  | | | | | | \\__ \\ |  __/ | |");
    Serial.println("/_/   \\_\\ |_| |_| |_| |___/  \\___| |_|");
}

void serialWelcomeInstructions() {
    Serial.println();
    Serial.println();
    Serial.print("Connect to wifi \"Amsel\" and visit ");
    Serial.print(AccessPointIP());
    Serial.print("/setup to get started.");  
}
