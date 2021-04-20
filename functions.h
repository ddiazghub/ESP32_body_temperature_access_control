#include "defines.h"

// Print line to display with position (x, y).
void printToDisplay(byte x, byte y, byte size, String line) {
  display.setCursor(x, y);  
  display.setTextSize(size);
  display.println(line);
}

// Start display.
void initDisplay() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  printToDisplay(25, 15, 1, "Termómetro infrarojo");
  printToDisplay(25, 35, 1, "Inicializando...");
  display.display();
  delay(2500);
}


// Print temperature to display with position (x, y).
void printTemperature(byte x, byte y, byte size, double temperature) {
  display.setCursor(x, y);  
  display.setTextSize(size);
  display.print(temperature);
  display.print((char)247);
  display.print("C");
}

// Delay while printing amount of seconds left at specified location and with specified size.
void countdown(int seconds, byte x, byte y, byte size) {
  while (seconds > 0) {
    printToDisplay(x, y, size, "" + seconds);
    delay(1000);
    seconds--;
  }
}

void openDoor() {

}