// Include variable definitions header file
#include "defines.h"

// Print line to display with position (x, y).
void printToDisplay(byte x, byte y, byte size, String line) {
  display.setCursor(x, y);  
  display.setTextSize(size);
  display.println(line);
  display.display();
}

// Start display.
void initDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(2500);
  display.clearDisplay();
  display.setTextColor(WHITE);
  printToDisplay(5, 20, 1, "Termometro infrarojo");
  printToDisplay(20, 30, 1, "Inicializando...");
  display.display();
  delay(3000);
}


// Print temperature to display with position (x, y).
void printTemperature(byte x, byte y, byte size, double temperature) {
  display.setCursor(x, y);  
  display.setTextSize(size);
  display.print(temperature);
  display.print(" ");
  display.print((char)247);
  display.print("C");
  display.display();
}

// Delay while printing amount of seconds left at specified location and with specified size.
void countdown(int seconds, byte x, byte y, byte size) {
  while (seconds > 0) {
    display.fillRect(x, y, 5, 5, SSD1306_BLACK);
    printToDisplay(x, y, size, String(seconds));
    delay(1000);
    seconds--;
  }
}

void openDoor() {
  servo.attach(SERVO_PIN_SIG);
  delay(500);
  servo.write(SERVO_TARGET);
  delay(7000);
  servo.write(SERVO_REST);
  servo.detach();
}