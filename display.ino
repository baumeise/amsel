#include "image_amsel.h"

#define STARTUP_DURATION 5000
#define HELLO_DURATION 5000+STARTUP_DURATION

#define DEFAULT_FONT ArialMT_Plain_10
#define STATUS_BAR_HEIGHT 10
#define DEFAULT_TEXT_ALIGN TEXT_ALIGN_CENTER_BOTH
#define DEFAULT_TEXT_POS_X (display.getWidth()-STATUS_BAR_HEIGHT)/2
#define DEFAULT_TEXT_POS_Y (display.getHeight()-STATUS_BAR_HEIGHT)/2

typedef void (*Function)(void);

void setup_display() {  
  display.init();
  display.flipScreenVertically();
}

// Draw functions
void drawText(String text) {
  display.setTextAlignment(DEFAULT_TEXT_ALIGN);
  display.setFont(DEFAULT_FONT);
  display.drawString(DEFAULT_TEXT_POS_X, \
                     DEFAULT_TEXT_POS_Y, \
                     text);
}

void drawImageAmsel() {
  display.drawXbm(18, 2, image_amsel_width, image_amsel_height, image_amsel_bits);
}

void drawProgressBarStartUp(int progress) {
  display.drawProgressBar(0, 54, 100, 8, progress);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 52, String(progress) + "%");
}

void drawHello() {
  drawText("Howdy, I am Amsel!");
}

void drawWifi() {
  drawText(String("") +\
            "Please connect to WI-FI"    +"\n"+\
            "\"Amsel\" and use password" +"\n"+\
            "\"passwort\".");
}

void drawIp() {
      drawText(String("") +\
            "Now visit" +"\n"+\
            AccessPointIP().toString() + "/setup" +"\n"+\
            "to get started.");
}

void drawOtaStart() {
  drawText("OTA Update");
}

void drawOtaProgress(unsigned int progress, unsigned int total) {
  display.drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
}

void drawOtaEnd() {
  drawText("Restart");
}

void drawOtaError(String message) {
    drawText(String("") +\
            "- OTA error -" +"\n"+\
            message +"\n"+\
            "- please reset -");
}

// Phase handling
void startUp() {
  drawImageAmsel();
  drawProgressBarStartUp((millis()*100) / STARTUP_DURATION);
}

void hello() {
  drawHello();
}

void main_display() {
  if (WiFi.softAPgetStationNum() == 0) {
    drawWifi();
  } else if (isOtaActive) {
    // wait for OTA and Reset
  } else {
    drawIp();
  }
}
void user_display() {
  drawText(userText);
}

// Loop handling
Function phase = startUp;
bool aliveOverlayState = false;

void loop_display() {
  // Display phase
  display.clear();
  phase();
  overlay();
  display.display();

  // Select phase
  if (userText.length() > 0) {
    phase = user_display;
  } else if (millis() > HELLO_DURATION) {       // Last phase
    phase = main_display;
  } else if (millis() > STARTUP_DURATION) {
    phase = hello;
  } else {                               // First Phase
    phase = startUp;
  }
}

void overlay() {
    aliveOverlayState = !aliveOverlayState;
    if (aliveOverlayState) {
      display.fillCircle(2, 2, 2);
    }
    if (WiFi.softAPgetStationNum() > 0) {
      display.drawXbm(118, 0, overlay_wifi_width, overlay_wifi_height, overlay_wifi_bits);
    }
}
