// Include Libraries
#include "includes.h"

// Pin Definitions
#define BUZZER_PIN_SIG	5
#define HCSR04_PIN_TRIG	4
#define HCSR04_PIN_ECHO	2
#define LEDG_PIN_VIN	12
#define LEDR_PIN_VIN	13
#define OLED_RST	17
#define OLED_DC	16
#define OLED_CS	33 // Unused
#define SERVO_PIN_SIG	14
#define BUZZER_CHANNEL 0 // PWM channel to be used with buzzer;
#define I2C_SDA 21
#define I2C_SCL 22 // I2C
#define RX 10
#define TX 10 // UART
#define MOSI 23
#define SPI_SCL 18 // SPI


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
};                          // Bitmap to display adafruit logo

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64    // OLED display dimensions
#ifndef SSD1306_128_64
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif                      // Checking if defined display height in library is correct
#define MAX_DISTANCE 5      // Max distance for taking temperature (cm)
#define TRIGGER 't'         // Trigger char for UART
#define EMISSIVITY 1      // Human body emissivity
#define THRESHOLD 38        // Temperature threshold for allowing access
#define SERVO_REST 20    // Resting position
#define SERVO_TARGET 150  // Target position to move to
double temp_amb;
double temp_body;           // Vars for storing ambient and body temperature


// object initialization
Buzzer buzzer(BUZZER_PIN_SIG, BUZZER_CHANNEL);
NewPing ultrasonic(HCSR04_PIN_TRIG, HCSR04_PIN_ECHO, 30);
Led ledG(LEDG_PIN_VIN);
Led ledR(LEDR_PIN_VIN);
Adafruit_SSD1306 display(OLED_DC, OLED_RST, OLED_CS);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Servo servo;