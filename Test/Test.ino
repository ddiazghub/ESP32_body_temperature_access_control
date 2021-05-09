
// Include Libraries
#include "./src/Lib/ESP32_Buzzer/Buzzer.h"
#include "./src/Lib/NewPing/NewPing.h"
#include "./src/Lib/Led/Led.h"
#include "Wire.h"
#include "SPI.h"
#include "./src/Lib/GFX/Adafruit_SSD1306.h"
#include "./src/Lib/Adafruit_MLX90614/Adafruit_MLX90614.h"
#include "./src/Lib/GFX/Adafruit_GFX.h"
#include "./src/Lib/ESP32_Servo/ESP32Servo.h"


// Pin Definitions
#define BUZZER_PIN_SIG	5
#define HCSR04_PIN_TRIG	4
#define HCSR04_PIN_ECHO	2
#define LEDG_PIN_VIN	12
#define LEDR_PIN_VIN	13
#define OLED128X64_PIN_RST	17
#define OLED128X64_PIN_DC	16
#define OLED128X64_PIN_CS	33
#define SERVOSM_PIN_SIG	14
#define BUZZER_CHANNEL 0

// Global variables and defines
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] = { 
  B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 
};

#define SSD1306_LCDHEIGHT 64
#ifndef SSD1306_128_64
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#define MAX_DISTANCE 40
const int servoSMRestPosition   = 20;  //Starting position
const int servoSMTargetPosition = 150; //Position when event is detected
double emissivity = 1;  // Idk
double threshold = 38.0;    // Temperature threshold for denying access

// object initialization
Buzzer buzzer(BUZZER_PIN_SIG, BUZZER_CHANNEL);
NewPing hcsr04(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO, MAX_DISTANCE);
Led ledG(LEDG_PIN_VIN);
Led ledR(LEDR_PIN_VIN);
Adafruit_SSD1306 display(OLED128X64_PIN_DC, OLED128X64_PIN_RST, OLED128X64_PIN_CS);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Servo servoSM;


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(115200);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    mlx.begin(); 
    display.begin(SSD1306_SWITCHCAPVCC);  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)

    servoSM.attach(SERVOSM_PIN_SIG);
    servoSM.write(servoSMRestPosition);
    delay(100);
    servoSM.detach();
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    if(menuOption == '1') {
        // Buzzer - Test Code
        // The buzzer will turn on and off for 500ms (0.5 sec)
        buzzer.error();
        delay(1000);
        buzzer.success();
        delay(1000);
        buzzer.siren();      
    
    } else if(menuOption == '2') {
        // Ultrasonic Sensor - HC-SR04 - Test Code
        // Read distance measurment from UltraSonic sensor           
        int hcsr04Dist = hcsr04.ping_cm();
        delay(500);
        Serial.print(F("Distance: ")); Serial.print(hcsr04Dist); Serial.println(F("[cm]"));
    } else if(menuOption == '3') {
        // LED - Basic Green 5mm - Test Code
        // The LED will turn on and fade till it is off
        ledG.on();
        delay(2000);                                      
        ledG.off();                        // 3. turns off
    } else if(menuOption == '4') {
        // LED - Basic Red 5mm - Test Code
        // The LED will turn on and fade till it is off
        ledR.on();
        delay(2000);                                         
        ledR.off();                        // 3. turns off
    }
    else if(menuOption == '5') {
        // Disclaimer: The Infrared Thermometer - MLX90614 is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
        double temp_amb = mlx.readAmbientTempC();
        double temp_body = mlx.readObjectTempC();
        Serial.print(F("Ambient temperature: ")); Serial.print(temp_amb); Serial.println(F(" °C"));
        Serial.print(F("Body temperature: ")); Serial.print(temp_body); Serial.println(F(" °C"));
        delay(300);
    }
    else if(menuOption == '6') {
        // Monochrome 1.3 inch 128x64 OLED graphic display - Test Code
        testPrint();
    }
    else if(menuOption == '7') {
        // Servo - Generic Metal Gear (Micro Size) - Test Code
        // The servo will rotate to target position and back to resting position with an interval of 500 milliseconds (0.5 seconds) 
        servoSM.attach(SERVOSM_PIN_SIG);         // 1. attach the servo to correct pin to control it.
        servoSM.write(servoSMTargetPosition);  // 2. turns servo to target position. Modify target position by modifying the 'ServoTargetPosition' definition above.
        delay(500);                              // 3. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        servoSM.write(servoSMRestPosition);    // 4. turns servo back to rest position. Modify initial position by modifying the 'ServoRestPosition' definition above.
        delay(500);                              // 5. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        servoSM.detach();                    // 6. release the servo to conserve power. When detached the servo will NOT hold it's position under stress.
    }
    
    if (millis() - time0 > timeout) {
        menuOption = menu();
    }
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu() {

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) Buzzer"));
    Serial.println(F("(2) Ultrasonic Sensor - HC-SR04"));
    Serial.println(F("(3) LED - Basic Green 5mm"));
    Serial.println(F("(4) LED - Basic Red 5mm"));
    Serial.println(F("(5) Infrared Thermometer - MLX90614"));
    Serial.println(F("(6) Monochrome 1.3 inch 128x64 OLED graphic display"));
    Serial.println(F("(7) Servo - Generic Metal Gear (Micro Size)"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available())  {
        char c = Serial.read();
        if (isAlphaNumeric(c)) {   
            if(c == '1') Serial.println(F("Now Testing Buzzer"));
            else if(c == '2') Serial.println(F("Now Testing Ultrasonic Sensor - HC-SR04"));
            else if(c == '3') Serial.println(F("Now Testing LED - Basic Green 5mm"));
            else if(c == '4') Serial.println(F("Now Testing LED - Basic Red 5mm"));
            else if(c == '5') Serial.println(F("Now Testing Infrared Thermometer - MLX90614 - note that this component doesn't have a test code"));
            else if(c == '6') Serial.println(F("Now Testing Monochrome 1.3 inch 128x64 OLED graphic display"));
            else if(c == '7') Serial.println(F("Now Testing Servo - Generic Metal Gear (Micro Size)"));
            else {
                Serial.println(F("illegal input!"));
                return 0;
            }
        time0 = millis();
        return c;
        }
    }

    return 0;
}

void testPrint() {
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  printToDisplay(5, 20, 1, "Termometro infrarojo");
  printToDisplay(20, 30, 1, "Inicializando...");
  delay(2000);

  display.clearDisplay();
  printToDisplay(0, 0, 1, "Temperatura ambiente:");
  printTemperature(17, 25, 2, 30);

  printToDisplay(0, 48, 1, "  Error: Temperatura   ambiente muy alta");
  delay(2000);

  display.clearDisplay();
  printToDisplay(0, 5, 1, " Posicionese a maximo ");
  display.setCursor(19, 15);
  display.print(10);
  display.print("cm del sensor");
  display.display();

  delay(2000);

  printToDisplay(30, 50, 1, "Esperando...");
  delay(2000);

  countdown(3, 62, 40, 1);

  display.clearDisplay();
  printToDisplay(25, 17, 1, "No se tomo la");
  printToDisplay(25, 27, 1, " Temperatura");
  printToDisplay(25, 37, 1, "correctamente");
  delay(2000);

  display.clearDisplay();
  printToDisplay(0, 0, 1, "Temperatura corporal:");
  printTemperature(17, 25, 2, 37);
  printToDisplay(0, 48, 1, " Su temperatura esta       muy alta");
  delay(2000);

  display.clearDisplay();
  printToDisplay(0, 0, 1, "Temperatura corporal:");
  printTemperature(17, 25, 2, 37);
  printToDisplay(32, 53, 1, "Bienvenido");
  delay(2000);

  display.clearDisplay();
  display.display();
}

// Print line to display with position (x, y).
void printToDisplay(byte x, byte y, byte size, String line) {
  display.setCursor(x, y);  
  display.setTextSize(size);
  display.println(line);
  display.display();
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
