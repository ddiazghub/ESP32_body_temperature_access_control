
// Include Libraries
#include "Arduino.h"
#include "Lib/Buzzer.h"
#include "Lib/HCSR04.h"
#include "Lib/LED.h"
#include "Wire.h"
#include "SPI.h"
#include "Lib/Adafruit_SSD1306.h"
#include "Lib/Adafruit_MLX90614.h"
#include "Lib/Adafruit_GFX.h"


// Pin Definitions
#define BUZZER_PIN_SIG	0
#define HCSR04_PIN_TRIG	13
#define HCSR04_PIN_ECHO	2
#define LEDG_PIN_VIN	14
#define LEDR_PIN_VIN	12
#define OLED_RST	15
#define TRIGGER 20

// Global variables and defines
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels

// object initialization
Buzzer buzzer(BUZZER_PIN_SIG);
HCSR04 ultrasonic(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO);
Led ledG(LEDG_PIN_VIN);
Led ledR(LEDR_PIN_VIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// define vars for testing menu
double temp_amb;
double temp_body;
double calibration = 2.36;
double limit = 38.0;
char menuOption = 0;
long time0;
int measureDistance = 10;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    Serial.begin(9600);
    pinMode(TRIGGER, INPUT);

    //Initialize I2C objects
    mlx.begin(); 
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //initialize with the I2C addr 0x3C (128x64)
    
    Serial.println("Temperature sensor and display initialized")
    
    // Setup display
    display.clearDisplay();
    oLed128x32.display();
    display.clearDisplay();
    display.setCursor(25,15);  
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("Termómetro infrarojo");
    display.setCursor(25,35);
    display.setTextSize(1);
    display.print("Inicializando");
    display.display();
    delay(2500);
}

// Main
void loop() 
{
    if (!digitalRead(TRIGGER)) {
      temp_amb = mlx.readAmbientTempC();

      Serial.print("Room Temp = ");
      Serial.println(temp_amb);

      display.clearDisplay();
      display.setCursor(10,0);  
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.println(" Temperatura ambiente: ");
    
      display.setCursor(10,40);
      display.setTextSize(3);
      display.print(temp_amb);
      display.print((char)247);
      display.print("C");

      if (temp_amb > limit) {
        ledR.on();
        buzzer.on();

        display.setCursor(0,53);
        display.setTextSize(1);
        display.println("Error: Temperatura ambiente muy alta");
      }

      continue;
    }

    display.clearDisplay();
    display.setCursor(10,20);  
    display.setTextSize(1);
    display.println(" Posicionese a máximo ");

    display.setCursor(10,30);  
    display.setTextSize(1);
    display.println(maxDistance + "cm del sensor.");

    display.setCursor(30,50);  
    display.setTextSize(1);
    display.println("Esperando...");

    while(ultrasonic.distance() > maxDistance) {
      Serial.prinln("Waiting...");
    }
  
    display.setCursor(10,40);
    display.setTextSize(3);
    display.print(temp_amb);
    display.print((char)247);
    display.print("C");

    temp_body = mlx.readObjectTempC();
    Serial.print("Body temp = ");
    Serial.println(temp_body);

    display.clearDisplay();
    display.setCursor(10,0);  
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println(" Temperatura corporal: ");
  
    display.setCursor(10,40);
    display.setTextSize(3);
    display.print(temp_body);
    display.print((char)247);
    display.print("C");

    if (temp_body > limit) {
      ledR.on();
      buzzer.on();

      display.setCursor(0,53);
      display.setTextSize(1);
      display.println("Su temperatura está muy alta");

      continue;
    }
}