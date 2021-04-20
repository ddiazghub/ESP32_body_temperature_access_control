// Include setup header file
// #include "src/setup.h"

// Include Libraries
#include <Arduino.h>
#include "src/Lib/Buzzer.h"
#include "src/Lib/HCSR04.h"
#include "src/Lib/LED.h"
#include <Wire.h>
#include <SPI.h>
#include "src/Lib/Adafruit_SSD1306.h"
#include "src/Lib/Adafruit_MLX90614.h"
#include "src/Lib/Adafruit_GFX.h"
#include <Servo.h>

// Pin Definitions
#define BUZZER_PIN_SIG	0
#define HCSR04_PIN_TRIG	13
#define HCSR04_PIN_ECHO	2
#define LEDG_PIN_VIN	14
#define LEDR_PIN_VIN	12
#define OLED_RST	15

// Global variables and defines
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64    // OLED display dimensions
double temp_amb;
double temp_body;           // Vars for storing ambient and body temperature
double calibration = 2.36;  // Idk
double threshold = 38.0;        // Temperature threshold for denying access
int maxDistance = 10;       // Max distance for taking temperature
byte trigger = 't';       // Trigger

// object initialization
Buzzer buzzer(BUZZER_PIN_SIG);
HCSR04 ultrasonic(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO);
Led ledG(LEDG_PIN_VIN);
Led ledR(LEDR_PIN_VIN);
Adafruit_SSD1306 display(OLED_RST);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Servo servo;

// Setup.
void setup() 
{
    Serial.begin(9600);

    //Initialize I2C objects
    mlx.begin(); 
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //I2C addr 0x3C.
    
    Serial.println("Temperature sensor and display initialized.");
    
    // Setup display
    initDisplay();
}

// Main
void loop() {
    // If trigger not received display room temperature continously.
    if (!Serial.available()) {
      temp_amb = mlx.readAmbientTempC();

      display.clearDisplay();
      
      printToDisplay(10, 0, 1, "Temperatura ambiente:");
      printTemperature(10, 40, 3, temp_amb);


      // If room temperature is higher than the threshold display err message.
      if (temp_amb > threshold) {
        ledR.on();
        buzzer.on();

        printToDisplay(0, 53, 1, "Error: Temperatura ambiente muy alta");
      }
      return;
    }

    byte serialInput = Serial.read();
    if (!serialInput == trigger) {
      return;
    };

    // Ask person to stand at 10 cm or less from sensor.
    display.clearDisplay();
    printToDisplay(10, 20, 1, " Posicionese a máximo ");
    printToDisplay(10, 30, 1, maxDistance + "cm del sensor.");


    // Wait
    printToDisplay(10,50, 1, "Esperando...");
    while(ultrasonic.distance() > maxDistance) {

    };

    countdown(3, 45, 50, 1);
    if (ultrasonic.distance() > maxDistance) {
      display.clearDisplay();
      printToDisplay(10, 20, 1, " No se tomó la temperatura correctamente ");
      delay(2000);
      return;
    };

    
    // Take temperature and send to RFID system.
    temp_body = mlx.readObjectTempC();
    Serial.print(temp_body);

    display.clearDisplay();
    printToDisplay(10, 0, 1, " Temperatura corporal: ");
    printTemperature(10, 40, 3, temp_body);


    // If temperature is over threshold light red LED, trigger buzzer, and print alert.
    if (temp_body > threshold) {
      ledR.on();
      buzzer.on();

      printToDisplay(0, 53, 1, "Su temperatura está muy alta");
      delay(2000);
      return;
    }

    ledG.on();
    printToDisplay(25, 53, 1, "Bienvenido");
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


// Print line to display with position (x, y).
void printToDisplay(byte x, byte y, byte size, String line) {
  display.setCursor(x, y);  
  display.setTextSize(size);
  display.println(line);
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