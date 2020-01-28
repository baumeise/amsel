WiFiUDP Udp;
#define INPUT_SIZE 30

void udp_routes(char* command, int arg) {
  Serial.println(command);
  if (strcmp(command, "forward") == 0) { 
    handleForward(arg);
  } else if (strcmp(command, "reverse") == 0) {
    handleBackward(arg);
  } else if (strcmp(command, "stop") == 0) {
    stop();
  } else if (strcmp(command, "right") == 0) {
    handleRight(arg);
  } else if (strcmp(command, "left") == 0) {
    handleLeft(arg);
  } else {
    Serial.print("Unsupported command: \"");
    Serial.print(command);
    Serial.print("\" with argument: ");
    Serial.println(arg);
  }
}

void setup_udp() {
  Udp.begin(UDP_port);
}

void loop_udp() {
  int noBytes = Udp.parsePacket();
  // Udp.remoteIP()
  // Udp.remotePort()

  if (noBytes && noBytes <= INPUT_SIZE) {
    // Get next command from UDP (add 1 for final 0)
    char input[INPUT_SIZE + 1];
    Udp.read(input, noBytes);
    // Add the final 0 to end the C string
    input[noBytes] = NULL;
    
    // Read each command pair 
    char* command = strtok(input, "&");
    while (command != NULL)
    {
        // Split the command in two values
        char* separator = strchr(command, ':');
        if (separator != NULL)
        {
            // Actually split the string in 2: replace ':' with 0
            *separator = NULL;
            char* cmd = command;
            ++separator;
            int arg = atoi(separator);
            
            // Do something with command and argument
            udp_routes(cmd, arg);
        }
        // Find the next command in input string
        command = strtok(NULL, "&");
    }
  }
}
